#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <chrono>
#include <unordered_map>

template <class Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

  Iterator begin() const { return begin_; }
  Iterator end() const { return end_; }

private:
  Iterator begin_, end_;
};


namespace traverses {

// Traverses the connected component in a breadth-first order
// from the vertex 'origin_vertex'.
// Refer to
// https://goo.gl/0qYXzC
// for the visitor events.
  template <class Vertex, class Graph, class Visitor>
  void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                          Visitor visitor);

  template <class Vertex, class Edge>
  class BfsVisitor {
  public:
    virtual void DiscoverVertex(Vertex /*vertex*/) {}
    virtual void ExamineEdge(const Edge & /*edge*/) {}
    virtual void ExamineVertex(Vertex /*vertex*/) {}
    virtual ~BfsVisitor() = default;
  };

}  // namespace traverses


namespace traverses {

  template<class Vertex, class Graph, class Visitor>
  void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                          Visitor visitor) {
    std::queue<Vertex> vertices_to_process({origin_vertex});
    std::unordered_set<Vertex> discovered_vertices = {origin_vertex};
    visitor.DiscoverVertex(origin_vertex);
    while (!vertices_to_process.empty()) {
      const Vertex current_vertex = vertices_to_process.front();
      vertices_to_process.pop();
      visitor.ExamineVertex(origin_vertex);

      for (const auto &edge : OutgoingEdges(graph, current_vertex)) {
        const Vertex target = GetTarget(graph, edge);
        if (discovered_vertices.count(target)) {
          continue;
        }
        discovered_vertices.insert(target);
        visitor.DiscoverVertex(target);
        visitor.ExamineEdge(edge);

        vertices_to_process.push(target);
      }
    }
  };

} // namespace traverses


namespace aho_corasick {

  struct AutomatonNode {
    AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}

    // Stores ids of strings which are ended at this node.
    std::vector<size_t> terminated_string_ids;

    // Stores tree structure of nodes.
    std::map<char, AutomatonNode> trie_transitions;

    // Stores cached transitions of the automaton, contains
    // only pointers to the elements of trie_transitions.
    std::map<char, AutomatonNode *> automaton_transitions_cache;
    AutomatonNode *suffix_link;
    AutomatonNode *terminal_link;
  };


  void PrintNode(const AutomatonNode* node) {
    std::cerr << "\nPRINT\n";
    std::cerr << "ids\n";
    for (auto id : node->terminated_string_ids) {
      std::cerr << id << " ";
    }
    std::cerr << "\n";
    std::cerr << "links:\n";
    for (auto& son : node->trie_transitions) {
      std::cerr << son.first << " " << &(son.second) << "\n";
    }
    std::cerr << "\n";
    std::cerr << "sufflink = " << node->suffix_link << "\n";
    for (auto& son : node->trie_transitions) {
      std::cerr << "print son: " << son.first << " " << &(son.second) << "\n";
      PrintNode(&(son.second));
    }
  }

// Returns a corresponding trie transition 'nullptr' otherwise.
  AutomatonNode *GetTrieTransition(AutomatonNode *node, char character);

// Returns an automaton transition, updates 'node->automaton_transitions_cache'
// if necessary.
// Provides constant amortized runtime.
  AutomatonNode *GetAutomatonTransition(AutomatonNode *node,
                                        const AutomatonNode *root,
                                        char character);



  namespace internal {

    class AutomatonGraph {
     public:
      struct Edge {
        Edge(AutomatonNode *source, AutomatonNode *target, char character)
            : source(source), target(target), character(character) {}

        AutomatonNode *source;
        AutomatonNode *target;
        char character;
      };
    };

    std::vector<typename AutomatonGraph::Edge> OutgoingEdges(
        const AutomatonGraph &, AutomatonNode *vertex) {
      std::vector<typename AutomatonGraph::Edge> edges;
      for (auto& target_it : vertex->trie_transitions) {
        edges.emplace_back(vertex, &target_it.second, target_it.first);
      }
      return edges;
    }

    AutomatonNode *GetTarget(const AutomatonGraph &,
                             const AutomatonGraph::Edge &edge) {
      return edge.target;
    }

    class SuffixLinkCalculator
        : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
     public:
      explicit SuffixLinkCalculator(AutomatonNode *root) : root_(root) {}

      void ExamineVertex(AutomatonNode *node) override;

      void ExamineEdge(const AutomatonGraph::Edge &edge) override;

    private:
      AutomatonNode *root_;
    };

    class TerminalLinkCalculator
        : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
     public:
      explicit TerminalLinkCalculator(AutomatonNode *root) : root_(root) {}


      void DiscoverVertex(AutomatonNode *node) override;

    private:
      AutomatonNode *root_;
    };
}  //  namespace internal


  void internal::SuffixLinkCalculator::ExamineVertex(AutomatonNode *node) {
    if (node == root_) {
      node->suffix_link = root_;
    }
  }

  void internal::SuffixLinkCalculator::ExamineEdge(const AutomatonGraph::Edge &edge) {

    if (edge.source == root_) {
      edge.target->suffix_link = root_;
    } else {
      edge.target->suffix_link = GetAutomatonTransition(edge.source->suffix_link,
                                                        root_,
                                                        edge.character);
    }
  }

  void internal::TerminalLinkCalculator::DiscoverVertex(AutomatonNode *node) {
    if (node == root_) {
      node->terminal_link = root_;
      return;
    }
    if (!node->terminated_string_ids.empty()) {
      node->terminal_link = node;
    } else {
      node->terminal_link = node->suffix_link->terminal_link;
    }
  }


  class NodeReference {
   public:
    NodeReference()
        : node_(nullptr), root_(nullptr) {}

    NodeReference(AutomatonNode *node, AutomatonNode *root)
        : node_(node), root_(root) {}

    NodeReference Next(char character) const;

    template <class Callback>
    void GenerateMatches(Callback on_match) const;

    bool IsTerminal() const;

    explicit operator bool() const { return node_ != nullptr; }

    bool operator==(NodeReference other) const;

    void Print() {
      std::cerr << "node refers to:" << node_ << "\n";
    }

  private:
    typedef std::vector<size_t>::const_iterator TerminatedStringIterator;
    typedef IteratorRange<TerminatedStringIterator> TerminatedStringIteratorRange;

    NodeReference TerminalLink() const;

    TerminatedStringIteratorRange TerminatedStringIds() const;

    AutomatonNode *node_;
    AutomatonNode *root_;
  };



  class AutomatonBuilder;

  class Automaton {
  public:
    /*
     * Чтобы ознакомиться с конструкцией =default, смотрите
     * https://goo.gl/jixjHU
     */
    Automaton() = default;

    Automaton(const Automaton &) = delete;
    Automaton &operator=(const Automaton &) = delete;

    NodeReference Root();

  private:
    AutomatonNode root_;

    friend class AutomatonBuilder;
  };









  class AutomatonBuilder {
  public:
    void Add(const std::string &string, size_t id);

    std::unique_ptr<Automaton> Build() {
      auto automaton = std::make_unique<Automaton>();
      BuildTrie(words_, ids_, automaton.get());
      BuildSuffixLinks(automaton.get());
      BuildTerminalLinks(automaton.get());
      return automaton;
    }

  private:
    static void BuildTrie(const std::vector<std::string> &words,
                          const std::vector<size_t> &ids, Automaton *automaton) {
      for (size_t i = 0; i < words.size(); ++i) {
        AddString(&automaton->root_, ids[i], words[i]);
      }
    }

    static void AddString(AutomatonNode *root, size_t string_id,
                          const std::string &string);

    static void BuildSuffixLinks(Automaton *automaton);

    static void BuildTerminalLinks(Automaton *automaton);

    std::vector<std::string> words_;
    std::vector<size_t> ids_;
  };

}  // namespace aho_corasick


template <class Callback>
void aho_corasick::NodeReference::GenerateMatches(Callback on_match) const {
  if (node_ != root_ && node_->terminated_string_ids.size() == 0) {
    NodeReference(node_->terminal_link, root_).GenerateMatches(on_match);
    return;
  }
  for (size_t element : node_->terminated_string_ids) {
    on_match(element);
  }
  if (node_ == root_) {
    return;
  }
  NodeReference(node_->suffix_link, root_).GenerateMatches(on_match);
}

aho_corasick::NodeReference aho_corasick::NodeReference::TerminalLink() const {
  return NodeReference(node_->terminal_link, root_);
}


aho_corasick::NodeReference::TerminatedStringIteratorRange
aho_corasick::NodeReference::TerminatedStringIds() const {
  return {node_->terminated_string_ids.begin(), node_->terminated_string_ids.end()};
}

aho_corasick::NodeReference aho_corasick::NodeReference::Next(char character) const {
  return NodeReference(GetAutomatonTransition(node_, root_, character), root_);
}

bool aho_corasick::NodeReference::IsTerminal() const {
  return !node_->terminated_string_ids.empty();
}

bool aho_corasick::NodeReference::operator==(NodeReference other) const {
  return node_ == other.node_;
}

aho_corasick::NodeReference aho_corasick::Automaton::Root() {
  return NodeReference(&root_, &root_);
}



aho_corasick::AutomatonNode * aho_corasick::GetTrieTransition(AutomatonNode *node,
                                                              char character) {
  auto transition = node->trie_transitions.find(character);
  if (transition != node->trie_transitions.end()) {
    return &transition->second;
  } else {
    return nullptr;
  }
}


aho_corasick::AutomatonNode * aho_corasick::GetAutomatonTransition(AutomatonNode *node,
                                                                   const AutomatonNode *root,
                                                                   char character) {
  const auto cache_transition_it = node->automaton_transitions_cache.emplace(character, nullptr);

  auto& cached_transition = cache_transition_it.first->second;

  if (!cache_transition_it.second) {
    return cached_transition;
  }

  const auto transition = node->trie_transitions.find(character);
  if (transition != node->trie_transitions.end()) {
    cached_transition = &transition->second;
    return &transition->second;
  }

  if (node != root) {
    const auto automaton_transition = GetAutomatonTransition(node->suffix_link,
                                                             root,
                                                             character);
    cached_transition = automaton_transition;
    return automaton_transition;
  } else {
    cached_transition = node;
    return node;
  }
}


void aho_corasick::AutomatonBuilder::Add(const std::string &string, size_t id) {
  words_.push_back(string);
  ids_.push_back(id);
}

void aho_corasick::AutomatonBuilder::AddString(AutomatonNode *root,
                                               size_t string_id,
                                               const std::string &string) {
  AutomatonNode *currentNode = root;
  for (char character : string) {
    currentNode = &currentNode->trie_transitions[character];
  }
  currentNode->terminated_string_ids.push_back(string_id);
}

void aho_corasick::AutomatonBuilder::BuildSuffixLinks(Automaton *automaton) {
  traverses::BreadthFirstSearch(&automaton->root_,
                                internal::AutomatonGraph(),
                                internal::SuffixLinkCalculator(&automaton->root_));
}

void aho_corasick::AutomatonBuilder::BuildTerminalLinks(Automaton *automaton) {
  traverses::BreadthFirstSearch(&automaton->root_,
                                internal::AutomatonGraph(),
                                internal::TerminalLinkCalculator(&automaton->root_));
}


// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template <class Predicate>
std::vector<std::string> Split(const std::string &string, Predicate is_delimiter);

// Wildcard is a character that may be substituted
// for any of all possible characters.
class WildcardMatcher {
public:
  WildcardMatcher() : number_of_words_(0), pattern_length_(0) {}

  WildcardMatcher static BuildFor(const std::string &pattern, char wildcard);

  // Resets the matcher. Call allows to abandon all data which was already
  // scanned,
  // a new stream can be scanned afterwards.
  void Reset();

  template <class Callback>
  void Scan(char character, Callback on_match);

private:
  void UpdateWordOccurrences();

  void ShiftWordOccurrencesCounters();

  // Storing only O(|pattern|) elements allows us
  // to consume only O(|pattern|) memory for matcher.
  std::deque<size_t> words_occurrences_by_position_;
  aho_corasick::NodeReference state_;
  size_t number_of_words_;
  size_t pattern_length_;
  std::unique_ptr<aho_corasick::Automaton> aho_corasick_automaton_;
};


template <class Predicate>
std::vector<std::string> Split(const std::string &string,
                               Predicate is_delimiter) {
  std::vector<std::string> splitted_strings;
  std::string cur_substring;
  for (const char character : string) {
    if (is_delimiter(character)) {
      splitted_strings.push_back(std::move(cur_substring));
      cur_substring.clear();
    } else {
      cur_substring.push_back(character);
    }
  }
  splitted_strings.push_back(std::move(cur_substring));
  return splitted_strings;
}


WildcardMatcher WildcardMatcher::BuildFor(const std::string &pattern, char wildcard) {
  const std::vector<std::string> subpatterns =
      Split(pattern, [wildcard](char ch) { return ch == wildcard; });
  aho_corasick::AutomatonBuilder builder;
  int shift = -1;
  for (int string_index = 0; string_index < subpatterns.size(); ++string_index) {
    shift += (subpatterns[string_index].size() + 1);
    builder.Add(subpatterns[string_index], shift);
  }
  WildcardMatcher wildcard_matcher;
  wildcard_matcher.number_of_words_ = subpatterns.size();
  wildcard_matcher.pattern_length_ = pattern.size();
  wildcard_matcher.aho_corasick_automaton_ = builder.Build();
  wildcard_matcher.Reset();

  return wildcard_matcher;
}

void WildcardMatcher::Reset() {
  state_ = aho_corasick_automaton_->Root();
  words_occurrences_by_position_.assign(pattern_length_, 0);
}

void PrintDeque(std::deque<size_t> deq) {
  std::cerr << "\nPRINT DEQUE \n";
  for (int element : deq)  {
    std::cerr << element << " ";
  }
  std::cerr << "\n";
}

template <class Callback>
void WildcardMatcher::Scan(char character, Callback on_match) {
  state_ = state_.Next(character);

  UpdateWordOccurrences();
  int deque_size = static_cast<int>(words_occurrences_by_position_.size());
  state_.GenerateMatches([this, &deque_size](int string_id) {
    ++words_occurrences_by_position_[deque_size - 1 - string_id];
  });

  if (words_occurrences_by_position_[0] == number_of_words_) {
    on_match();
  }
  ShiftWordOccurrencesCounters();
}

void WildcardMatcher::UpdateWordOccurrences() {
  words_occurrences_by_position_.push_back(0);
}

void WildcardMatcher::ShiftWordOccurrencesCounters() {
  words_occurrences_by_position_.pop_front();
}


std::string ReadString(std::istream &input_stream);

// Returns positions of the first character of an every match.
std::vector<size_t> FindFuzzyMatches(const std::string &pattern_with_wildcards,
                                     const std::string &text, char wildcard);

void Print(const std::vector<size_t> &sequence);

// void TestAll();

int main(int argc, char *argv[]) {

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  if (argc > 1 && strcmp(argv[1], "--test") == 0) {
    // TestAll();
    return 0;
  }

  const char wildcard = '?';
  const std::string pattern_with_wildcards = ReadString(std::cin);
  const std::string text = ReadString(std::cin);
  Print(FindFuzzyMatches(pattern_with_wildcards, text, wildcard));
  return 0;
}


std::vector<size_t> FindFuzzyMatches(const std::string &pattern_with_wildcards,
                                     const std::string &text, char wildcard) {
  WildcardMatcher wildcardMatcher = WildcardMatcher::BuildFor(pattern_with_wildcards, wildcard);
  std::vector<size_t> occurrences;
  std::string ntext = '#' + text;
  int pattern_size = pattern_with_wildcards.size();
  for (size_t char_index = 0; char_index < ntext.size(); ++char_index) {
    wildcardMatcher.Scan(ntext[char_index],
                         [&occurrences, char_index, pattern_size]() {
                           occurrences.push_back(char_index - pattern_size);
                         }
    );
  }
  return occurrences;
}

std::string ReadString(std::istream &input_stream) {
  std::string string;
  input_stream >> string;
  return string;
}

void Print(const std::vector<size_t> &sequence) {
  std::cout << sequence.size() << "\n";
  for (size_t element : sequence) {
    std::cout << element << " ";
  }
  std::cout << "\n";
}
