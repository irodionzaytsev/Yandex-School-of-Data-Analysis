/*
 * Интерфейс прокомментирован с целью объяснить,
 * почему он написан так, а не иначе. В реальной жизни
 * так никто никогда не делает. Комментарии к коду,
 * которые остались бы в его рабочем варианте, заданы
 * с помощью команды однострочного комментария // и написаны
 * на английском языке, как рекомендуется.
 * Остальные комментарии здесь исключительно в учебных целях.
 */

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
#include <unordered_set>

/*
 * Часто в c++ приходится иметь дело с парой итераторов,
 * которые представляют из себя полуинтервал. Например,
 * функция std:sort принимает пару итераторов, участок
 * между которыми нужно отсортировать. В с++11 появился
 * удобный range-based for, который позволяет итерироваться
 * по объекту, для которого определены функции std::begin
 * и std::end (например, это объекты: массив фиксированного
 * размера, любой объект, у которого определены методы
 * begin() и end()). То есть удобный способ итерироваться
 * по std::vector такой:
 * for (const std::string& string: words).
 * Однако, для некоторых объектов существует не один способ
 * итерироваться по ним. Например std::map: мы можем
 * итерироваться по парам объект-значение (как это сделает
 * for(...: map)), или мы можем итерироваться по ключам.
 * Для этого мы можем сделать функцию:
 * IteratorRange<...> keys(const std::map& map),
 * которой можно удобно воспользоваться:
 * for(const std::string& key: keys(dictionary)).
 */
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

/*
 * Для начала мы рекомендуем ознакомиться с общей
 * концепцией паттерна проектирования Visitor:
 * https://goo.gl/oZGiYl
 * Для применения Visitor'а к задаче обхода графа
 * можно ознакомиться с
 * https://goo.gl/5gjef2
 */
// See "Visitor Event Points" on
// https://goo.gl/wtAl0y
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
    std::queue<Vertex> vertices;
    std::unordered_set<Vertex> discovered_vertices;
    vertices.push(origin_vertex);
    discovered_vertices.insert(origin_vertex);
    visitor.DiscoverVertex(origin_vertex);
    while (!vertices.empty()) {
      Vertex current_vertex = vertices.front();
      vertices.pop();
      visitor.ExamineVertex(origin_vertex);

      for (const auto &edge : OutgoingEdges(graph, current_vertex)) {
        Vertex target = GetTarget(graph, edge);
        if (discovered_vertices.count(target)) {
          continue;
        }
        discovered_vertices.insert(target);
        visitor.DiscoverVertex(target);
        visitor.ExamineEdge(edge);

        vertices.push(target);
      }
    }
  };

}









namespace aho_corasick {

  struct AutomatonNode {
    AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}

    // Stores ids of strings which are ended at this node.
    std::vector<size_t> terminated_string_ids;
    // Stores tree structure of nodes.
    std::map<char, AutomatonNode> trie_transitions;
    /*
     * Обратите внимание, что std::set/std::map/std::list
     * при вставке и удалении неинвалидируют ссылки на
     * остальные элементы контейнера. Но стандартные контейнеры
     * std::vector/std::string/std::deque таких гарантий не
     * дают, поэтому хранение указателей на элементы этих
     * контейнеров крайне не рекомендуется.
     */
    // Stores cached transitions of the automaton, contains
    // only pointers to the elements of trie_transitions.
    std::map<char, AutomatonNode *> automaton_transitions_cache;
    AutomatonNode *suffix_link;
    AutomatonNode *terminal_link;
  };


  void printNode(AutomatonNode* node) {
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
      printNode(&(son.second));
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
        edges.push_back(AutomatonGraph::Edge(vertex, &target_it.second, target_it.first));
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
    if (node->terminated_string_ids.size() > 0) {
      node->terminal_link = node;
    } else {
      node->terminal_link = node->suffix_link->terminal_link;
    }
  }



/*
 * Объясним задачу, которую решает класс NodeReference.
 * Класс Automaton представляет из себя неизменяемый объект
 * (https://goo.gl/4rSP4f),
 * в данном случае, это означает, что единственное действие,
 * которое пользователь может совершать с готовым автоматом,
 * это обходить его разными способами. Это значит, что мы
 * должны предоставить пользователю способ получить вершину
 * автомата и дать возможность переходить между вершинами.
 * Одним из способов это сделать -- это предоставить
 * пользователю константный указатель на AutomatonNode,
 * а вместе с ним константый интерфейс AutomatonNode. Однако,
 * этот вариант ведет к некоторым проблемам.
 * Во-первых, этот же интерфейс AutomatonNode мы должны
 * использовать и для общения автомата с этим внутренним
 * представлением вершины. Так как константная версия
 * этого же интерфейса будет доступна пользователю, то мы
 * ограничены в добавлении функций в этот константный
 * интерфейс (не все функции, которые должны быть доступны
 * автомату должны быть доступны пользователю). Во-вторых,
 * так как мы используем кэширование при переходе по символу
 * в автомате, то условная функция getNextNode не может быть
 * константной (она заполняет кэш переходов). Это значит,
 * что мы лишены возможности добавить функцию "перехода
 * между вершинами" в константный интерфейс (то есть,
 * предоставить ее пользователю константного указателя на
 * AutomatonNode).
 * Во избежание этих проблем, мы создаем отдельный
 * класс, отвечающий ссылке на вершину, который предоставляет
 * пользователю только нужный интерфейс.
 */
  class NodeReference {
  public:
    NodeReference() : node_(nullptr), root_(nullptr) {}

    NodeReference(AutomatonNode *node, AutomatonNode *root)
      : node_(node), root_(root) {}

    NodeReference Next(char character) const;

    /*
     * В этом случае есть два хороших способа получить
     * результат работы этой функции:
     * добавить параметр типа OutputIterator, который
     * последовательно записывает в него id найденных
     * строк, или же добавить параметр типа Callback,
     * который будет вызван для каждого такого id.
     * Чтобы ознакомиться с этими концепциями лучше,
     * смотрите ссылки:
     * https://goo.gl/2Kg8wE
     * https://goo.gl/OaUB4k
     * По своей мощности эти способы эквивалентны. (см.
     * https://goo.gl/UaQpPq)
     * Так как в интерфейсе WildcardMatcher мы принимаем
     * Callback, то чтобы пользоваться одним и тем же средством
     * во всех интерфейсах, мы и здесь применяем Callback. Отметим,
     * что другие способы, как например, вернуть std::vector с
     * найденными id, не соответствуют той же степени гибкости, как
     * 2 предыдущие решения (чтобы в этом убедиться представьте
     * себе, как можно решить такую задачу: создать std::set
     * из найденных id).
     */
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
  return TerminatedStringIteratorRange(node_->terminated_string_ids.begin(),
                                       node_->terminated_string_ids.end());
}

aho_corasick::NodeReference aho_corasick::NodeReference::Next(char character) const {
  return NodeReference(GetAutomatonTransition(node_, root_, character), root_);
}

bool aho_corasick::NodeReference::IsTerminal() const {
  return node_->trie_transitions.size() > 0;
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
  const auto cache_transition = node->automaton_transitions_cache.find(character);
  if (cache_transition != node->automaton_transitions_cache.end()) {
    return cache_transition->second;
  }

  const auto transition = node->trie_transitions.find(character);
  if (transition != node->trie_transitions.end()) {
    return &transition->second;
  }

  if (node != root) {
    const auto automaton_transition = GetAutomatonTransition(node->suffix_link,
                                                       root,
                                                       character);
    node->automaton_transitions_cache[character] = automaton_transition;
    return automaton_transition;
  } else {
    node->automaton_transitions_cache[character] = node;
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
  /*
  std::queue<AutomatonNode*> nodes;
  std::queue<AutomatonNode*> node_fathers;
  std::queue<char> edge_characters;
  nodes.push(&automaton->root_);
  node_fathers.push(nullptr);
  edge_characters.push('#');
  while (!nodes.empty()) {
    AutomatonNode* currentNode = nodes.front();
    AutomatonNode* currentNodeFather = node_fathers.front();
    char father_character = edge_characters.front();
    nodes.pop();
    node_fathers.pop();
    edge_characters.pop();
    if (currentNode == &automaton->root_ || currentNodeFather == &automaton->root_) {
      currentNode->suffix_link = &automaton->root_;
    } else {
      currentNode->suffix_link = GetAutomatonTransition(currentNodeFather->suffix_link,
                                                        &automaton->root_,
                                                        father_character);
    }

    for (auto vertex_it = currentNode->trie_transitions.begin();
         vertex_it != currentNode->trie_transitions.end();
         ++vertex_it) {
      AutomatonNode *nextNode = &vertex_it->second;
      nodes.push(nextNode);
      node_fathers.push(currentNode);
      edge_characters.push(vertex_it->first);
    }
  }
   */

}

void aho_corasick::AutomatonBuilder::BuildTerminalLinks(Automaton *automaton) {
  traverses::BreadthFirstSearch(&automaton->root_,
                                internal::AutomatonGraph(),
                                internal::TerminalLinkCalculator(&automaton->root_));
  /*
  std::queue<AutomatonNode*> nodes;
  nodes.push(&automaton->root_);
  automaton->root_.terminal_link = &automaton->root_;
  while (!nodes.empty()) {
    AutomatonNode* currentNode = nodes.front();
    nodes.pop();
    if (currentNode->terminated_string_ids.size() > 0) {
      currentNode->terminal_link = currentNode;
    } else {
      currentNode->terminal_link = currentNode->suffix_link->terminal_link;
    }

    for (auto vertex_it = currentNode->trie_transitions.begin();
         vertex_it != currentNode->trie_transitions.end();
         ++vertex_it) {
      AutomatonNode *nextNode = &vertex_it->second;
      nodes.push(nextNode);
    }
  }
   */
}























































// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template <class Predicate>
std::vector<std::string> Split(const std::string &string,
                               Predicate is_delimiter);

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

  /* В данном случае Callback -- это функция,
   * которая будет вызвана при наступлении
   * события "суффикс совпал с шаблоном".
   * Почему мы выбрали именно этот способ сообщить
   * об этом событии? Можно рассмотреть альтернативы:
   * вернуть bool из Scan, принять итератор и записать
   * в него значение. В первом случае, значение bool,
   * возвращенное из Scan, будет иметь непонятный
   * смысл. True -- в смысле все считалось успешно?
   * True -- произошло совпадение? В случае итератора,
   * совершенно не ясно, какое значение туда  записывать
   * (подошедший суффикс, true, ...?). Более того, обычно,
   * если при сканировании потока мы наткнулись на
   * совпадение, то нам нужно как-то на это отреагировать,
   * это действие и есть наш Callback on_match.
   */
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
  std::vector<std::string> splited_strings;
  std::string cur_sub_string = "";
  for (char character : string) {
    if (is_delimiter(character)) {
      splited_strings.push_back(cur_sub_string);
      cur_sub_string = "";
    } else {
      cur_sub_string += character;
    }
  }
  if (is_delimiter(string.back()) || cur_sub_string.length() > 0) {
    splited_strings.push_back(cur_sub_string);
  }

  return splited_strings;
}


WildcardMatcher WildcardMatcher::BuildFor(const std::string &pattern, char wildcard) {
  std::vector<std::string> splited_strings = Split(pattern,
                                                   [wildcard](char ch) { return ch == wildcard; });
  aho_corasick::AutomatonBuilder builder;
  int shift = -1;
  for (int string_index = 0; string_index < splited_strings.size(); ++string_index) {
    shift += (splited_strings[string_index].size() + 1);
    builder.Add(splited_strings[string_index], shift);
  }
  WildcardMatcher wildcardMatcher;
  wildcardMatcher.number_of_words_ = splited_strings.size();
  wildcardMatcher.pattern_length_ = pattern.size();
  wildcardMatcher.aho_corasick_automaton_ = builder.Build();
  wildcardMatcher.Reset();

  return wildcardMatcher;
}

void WildcardMatcher::Reset() {
  state_ = aho_corasick_automaton_->Root();
  words_occurrences_by_position_.clear();
  words_occurrences_by_position_.push_back(0);
}

void printDeque(std::deque<size_t> deq) {
  std::cerr << "\nPRINT DEQUE \n";
  for (int element : deq)  {
    std::cerr << element << " ";
  }
  std::cerr << "\n";
}

template <class Callback>
void WildcardMatcher::Scan(char character, Callback on_match) {
  state_ = state_.Next(character);
  std::vector<int> terminated_ids;
  state_.GenerateMatches([&terminated_ids](int string_id) { terminated_ids.push_back(string_id); });
  UpdateWordOccurrences();
  int deque_size = static_cast<int>(words_occurrences_by_position_.size());
  for (int string_id : terminated_ids) {
    if (string_id < deque_size) {
      ++words_occurrences_by_position_[deque_size - 1 - string_id];
    }
  }

  if (words_occurrences_by_position_[0] == number_of_words_ && deque_size == pattern_length_ + 1) {
    on_match();
  }
  ShiftWordOccurrencesCounters();
}

void WildcardMatcher::UpdateWordOccurrences() {
  words_occurrences_by_position_.push_back(0);
}

void WildcardMatcher::ShiftWordOccurrencesCounters() {
  if (words_occurrences_by_position_.size() > pattern_length_) {
    words_occurrences_by_position_.pop_front();
  }
}




std::string ReadString(std::istream &input_stream);

// Returns positions of the first character of an every match.
std::vector<size_t> FindFuzzyMatches(const std::string &pattern_with_wildcards,
                                     const std::string &text, char wildcard);

void Print(const std::vector<size_t> &sequence);

void TestAll();

int main(int argc, char *argv[]) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);

  if (argc > 1 && strcmp(argv[1], "--test") == 0) {
    TestAll();
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



std::vector<std::string> BohrStrings(int count, int len) {
  std::vector<std::string> result;
  std::string random_symbols;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < len; ++j) {
      random_symbols += ('a' + (rand() % 2));
      // random_symbols += 'a';
    }
    result.push_back(random_symbols.substr(0, (count + 1) * len));
  }
  // std::cerr << "result = " << random_symbols << "\n";
  return result;
}

std::vector<std::string> RevBohrStrings(int count, int len) {
  std::vector<std::string> result;
  std::string random_symbols;
  for (int i = 0; i < count; ++i) {
    for (int j = 0; j < len; ++j) {
      random_symbols += ('a' + (rand() % 32));
      // random_symbols += 'a';
      // random_symbols += "a";
    }
  }

  for (int j = 0; j < count; ++j) {
    result.push_back(random_symbols.substr(len * j, count * len - count * j));
  }
  // std::cerr << "result = " << random_symbols << "\n";
  return result;
}

std::vector<std::string> FlowerString(int count, int len) {
  std::vector<std::string> result;
  std::string init_string;
  for (int i = 0; i < len; ++i) {
    init_string += 'a';
  }
  for (int i = 0; i < count; ++i) {
    std::string cur_string;
    for (int j = 0; j < len; ++j) {
      // random_symbols += ('a' + (rand() % 2));
      cur_string += ('a' + rand() % 2);
    }
    result.push_back(init_string + cur_string);
  }
  // std::cerr << "result = " << random_symbols << "\n";
  return result;
}

std::string BohrTest(int len) {
  std::string str;
  for (int i = 0; i < len; ++i) {
    str += ('a' + rand() % 32);
  }
  return str;
}



// ===== TESTING ZONE =====

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vector) {
  std::copy(vector.begin(), vector.end(), std::ostream_iterator<T>(os, " "));
  return os;
}

class TestNotPassedException : public std::runtime_error {
public:
  explicit TestNotPassedException(const char *what)
    : std::runtime_error(what) {}

  explicit TestNotPassedException(const std::string &what)
    : std::runtime_error(what.c_str()) {}
};

#define REQUIRE_EQUAL(first, second)                                         \
  do {                                                                       \
    auto firstValue = (first);                                               \
    auto secondValue = (second);                                             \
    if (firstValue != secondValue) {                                         \
      std::ostringstream oss;                                                \
      oss << "Require equal failed: " << #first << " != " << #second << " (" \
          << firstValue << " != " << secondValue << ")\n";                   \
      throw TestNotPassedException(oss.str());                               \
    }                                                                        \
  } while (false)

void TestBfs();
void TestSplit();

void TestAll() {
  {
    aho_corasick::AutomatonBuilder builder;
    builder.Add("suffix", 1);
    builder.Add("ffix", 2);
    builder.Add("ix", 3);
    builder.Add("abba", 4);

    std::unique_ptr<aho_corasick::Automaton> automaton = builder.Build();

    const std::string text = "let us find some suffix";
    aho_corasick::NodeReference node = automaton->Root();
    for (char ch : text) {
      node = node.Next(ch);
    }
    std::vector<size_t> string_ids;

    node.GenerateMatches(
      [&string_ids](size_t string_id) { string_ids.push_back(string_id); });
    std::sort(string_ids.begin(), string_ids.end());

    REQUIRE_EQUAL(string_ids, std::vector<size_t>({1, 2, 3}));
  }
  {
    aho_corasick::AutomatonBuilder builder;
    std::vector<std::string> bohr_strings = RevBohrStrings(10, 2000);

    std::cerr << "bohr_stirng.size() = " << bohr_strings.size() << "\n";

    for (auto bohr_string : bohr_strings) {
       // std::cerr << bohr_string << " ";
    }

    for (int i = 0; i < bohr_strings.size(); ++i) {
      std::string bohr_string = bohr_strings[i];
      builder.Add(bohr_string, i);
    }

    std::unique_ptr<aho_corasick::Automaton> automaton = builder.Build();

    const std::string text = BohrTest(100000);

    auto start = std::chrono::high_resolution_clock::now();



    aho_corasick::NodeReference node = automaton->Root();
    for (char ch : text) {
      node = node.Next(ch);
    }
    auto nexts_time = std::chrono::high_resolution_clock::now();
    std::cerr <<
              std::chrono::duration_cast<std::chrono::microseconds>(nexts_time - start).count()
              << "--nexts \n";

    std::vector<size_t> string_ids;

    node.GenerateMatches(
      [&string_ids](size_t string_id) { string_ids.push_back(string_id); });
    std::sort(string_ids.begin(), string_ids.end());

    // REQUIRE_EQUAL(string_ids, std::vector<size_t>({1, 2, 3}));
  }
  {
    WildcardMatcher matcher = WildcardMatcher::BuildFor("a?c?", '?');

    {
      std::vector<size_t> occurrences;
      //                              012345678901234
      const std::string first_text = "abcaaccxaxcxacc";
      for (size_t i = 0; i < first_text.size(); ++i) {
        matcher.Scan(first_text[i],
                     [&occurrences, i]() { occurrences.push_back(i); });
      }
      REQUIRE_EQUAL(occurrences, std::vector<size_t>({3, 6, 7, 11}));
    }
    {
      matcher.Reset();
      std::vector<size_t> occurrences;
      const std::string second_text = "xyzadcc";
      for (size_t i = 0; i < second_text.size(); ++i) {
        matcher.Scan(second_text[i],
                     [&occurrences, i]() { occurrences.push_back(i); });
      }

      REQUIRE_EQUAL(occurrences, std::vector<size_t>({6}));
    }
  }
  TestSplit();
  TestBfs();
  std::cerr << "Tests are passed!\n";
}

void TestSplit() {
  {
    auto is_dash = [](char ch) { return ch == '-'; };
    {
      REQUIRE_EQUAL(Split("a--b-cd-e", is_dash),
                    std::vector<std::string>({"a", "", "b", "cd", "e"}));
    }
    { REQUIRE_EQUAL(Split("-", is_dash), std::vector<std::string>({"", ""})); }
    {
      REQUIRE_EQUAL(Split("--abc--", is_dash),
                    std::vector<std::string>({"", "", "abc", "", ""}));
    }
    { REQUIRE_EQUAL(Split("ab", is_dash), std::vector<std::string>({"ab"})); }
  }
  {
    auto True = [](char /*ch*/) { return true; };
    {
      std::string s = "2f1tgyhnjd";
      // empty string before each character and one after the string
      REQUIRE_EQUAL(Split(s, True).size(), s.size() + 1);
    }
  }
}

namespace test_bfs {

  typedef std::vector<int> VerticesList;

  struct Graph {
    std::vector<VerticesList> adjacent_vertices;
  };

  int GetTarget(const Graph & /*graph*/, int edge) { return edge; }

  IteratorRange<VerticesList::const_iterator> OutgoingEdges(const Graph &graph,
                                                            int vertex) {
    return IteratorRange<VerticesList::const_iterator>(
      graph.adjacent_vertices[vertex].begin(),
      graph.adjacent_vertices[vertex].end());
  }
}  // namespace test_bfs


void TestBfs() {
  test_bfs::Graph graph;
  graph.adjacent_vertices.resize(4);
  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int d = 3;
  graph.adjacent_vertices[a].push_back(b);
  graph.adjacent_vertices[b].push_back(c);
  graph.adjacent_vertices[c].push_back(d);
  graph.adjacent_vertices[d].push_back(b);
  traverses::BreadthFirstSearch(a, graph, traverses::BfsVisitor<int, int>());
  // BreadthFirstSearch should not hang on a graph with a cycle
}
