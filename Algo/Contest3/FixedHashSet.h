#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <set>

const int64_t kDefaultPrimeModule = 2000000011;
const int64_t kMaxValue = 1000000000;
const int64_t kDispersion = 2;

std::vector<int> ReadNumbers(std::istream &in_stream = std::cin) {
  int set_numbers_size;
  in_stream >> set_numbers_size;
  std::vector<int> set_numbers;
  set_numbers.reserve(set_numbers_size);
  for (int i = 0;i < set_numbers_size; ++i) {
    int element;
    in_stream >> element;
    set_numbers.push_back(element);
  }
  return set_numbers;
}

std::vector<int> ReadQueries(std::istream &in_stream = std::cin) {
  int queries_size;
  in_stream >> queries_size;
  std::vector<int> queries;
  queries.reserve(queries_size);
  for (int i = 0; i < queries_size; ++i) {
    int element;
    in_stream >> element;
    queries.push_back(element);
  }
  return queries;
}

void WriteEmulatedQueryResult(const std::vector<bool> &emulated_result,
                              std::ostream &out_stream = std::cout) {
  for (auto answer : emulated_result) {
    if (answer) {
      out_stream << "Yes\n";
    } else {
      out_stream << "No\n";
    }
  }
}

class HashFunction {
 public:
  HashFunction() {}

  HashFunction(int64_t seed, int64_t shift, int64_t prime_module, int64_t bucket_size);

  int64_t operator() (int element) const;

  int64_t GetBucketSize() const;

  static HashFunction GenerateHashFunction(int64_t prime_module, int64_t bucket_size);

 private:
  int64_t seed_;
  int64_t shift_;
  int64_t bucket_size_;
  int64_t prime_module_;
};

template <typename T>
class Optional {
 public:
  Optional()
    : has_value_(false) {}

  void SetValue(const T& value) {
    has_value_ = true;
    value_ = value;
  }

  const T& GetValue() const {
    return value_;
  }

  bool HasValue() const {
    return has_value_;
  }

 private:
  bool has_value_;
  T value_;
};

class Bucket {
 public:
  Bucket();

  void Initialize(const std::vector<int>& elements);

  bool Contains(int element) const;

 private:
  static HashFunction PickUpHashFunction(const std::vector<int>& elements);

  static bool CheckHashFunction(const std::vector<int>& elements,
                                const HashFunction hash_function);

  void AssignBucket(const std::vector<int>& elements);

  std::vector<Optional<int>> hashed_numbers_;
  HashFunction hash_function_;
};

class FixedSet {
 public:
  FixedSet();

  void Initialize(const std::vector<int>& set_numbers);

  bool Contains(int element) const;

 private:
  static HashFunction PickUpHashFunction(const std::vector<int>& set_numbers);

  static bool CheckHashFunction(const HashFunction& hash_function,
                                const std::vector<int>& set_numbers,
                                const int buckets_count);

  void AssignBuckets(const std::vector<int>& set_numbers);

  std::vector<Bucket> buckets_;
  HashFunction hash_function_;
};

std::vector<bool> EmulateSetQueries(const std::vector<int>& set_numbers,
                                    const std::vector<int>& queries) {
  FixedSet fixedSet;
  fixedSet.Initialize(set_numbers);
  std::vector<bool> result;
  result.reserve(queries.size());
  for (int element : queries) {
    result.push_back(fixedSet.Contains(element));
  }
  return result;
}

HashFunction GenerateHashFunction(int64_t prime_module,
                                  int64_t bucket_size,
                                  std::default_random_engine& generator) {
  int64_t seed = std::uniform_int_distribution<int>(1, prime_module - 1)(generator);
  int64_t shift = std::uniform_int_distribution<int>(0, prime_module - 1)(generator);
  return HashFunction(seed, shift, prime_module, bucket_size);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::vector<int> set_numbers = ReadNumbers();
  std::vector<int> queries = ReadQueries();
  const std::vector<bool> emulated_result = EmulateSetQueries(set_numbers, queries);
  WriteEmulatedQueryResult(emulated_result);
  return 0;
}

HashFunction::HashFunction(int64_t seed, int64_t shift, int64_t prime_module, int64_t bucket_size)
  : seed_(seed), shift_(shift), prime_module_(prime_module), bucket_size_(bucket_size) {}

int64_t HashFunction::operator () (int element) const {
  return ((seed_ * (int64_t) element + shift_) % prime_module_) % bucket_size_;
}

int64_t HashFunction::GetBucketSize() const {
  return bucket_size_;
}

std::vector<int64_t> BucketSizes(const HashFunction& hash_function,
                                 const std::vector<int>& set_numbers,
                                 const int count_buckets) {
  std::vector<int64_t> bucket_sizes(count_buckets, 0);
  for (int number : set_numbers) {
    number += kMaxValue;
    int64_t index = hash_function(number);
    ++bucket_sizes[index];
  }
  return bucket_sizes;
}

int64_t CalculateSecondMoment(const std::vector<int64_t> &buckets_sizes) {
  int64_t sum_of_squares = 0;
  for (int64_t bucket_size : buckets_sizes) {
    sum_of_squares += bucket_size * bucket_size;
  }
  return sum_of_squares;
}

Bucket::Bucket() {}

bool Bucket::CheckHashFunction(const std::vector<int> &elements,
                               const HashFunction hash_function) {
  size_t bucket_size = elements.size() * elements.size();
  bool success_flag = true;
  std::vector<int64_t> cell_sizes(bucket_size, 0);
  for (int element : elements) {
    int64_t index = hash_function(element);
    ++cell_sizes[index];
    if (cell_sizes[index] > 1) {
      success_flag = false;
      break;
    }
  }
  return success_flag;
}

HashFunction Bucket::PickUpHashFunction(const std::vector<int> &elements) {
  size_t bucket_size = elements.size() * elements.size();
  HashFunction hash_function;
  bool success_flag = false;
  std::default_random_engine generator;
  do {
    hash_function = GenerateHashFunction(kDefaultPrimeModule, bucket_size, generator);
    success_flag = Bucket::CheckHashFunction(elements, hash_function);
  } while (!success_flag);
  return hash_function;
}

void Bucket::AssignBucket(const std::vector<int>& elements) {
  size_t bucket_size = elements.size() * elements.size();
  hashed_numbers_.resize(bucket_size);

  for (int element : elements) {
    int64_t index = hash_function_(element);
    hashed_numbers_[index].SetValue(element);
  }
}

void Bucket::Initialize(const std::vector<int>& elements) {
  hash_function_ = Bucket::PickUpHashFunction(elements);
  AssignBucket(elements);
}

bool Bucket::Contains(int element) const {
  if (hash_function_.GetBucketSize() == 0) {
    return false;
  }
  int64_t index = hash_function_(element);

  return hashed_numbers_[index].HasValue() && hashed_numbers_[index].GetValue() == element;
}

FixedSet::FixedSet() {}

bool FixedSet::CheckHashFunction(const HashFunction& hash_function,
                                 const std::vector<int>& set_numbers,
                                 const int buckets_count) {
  auto buckets_sizes = BucketSizes(hash_function, set_numbers, buckets_count);
  return (CalculateSecondMoment(buckets_sizes) <= kDispersion * buckets_count);
}

HashFunction FixedSet::PickUpHashFunction(const std::vector<int>& set_numbers) {
  size_t buckets_count = set_numbers.size();
  HashFunction hash_function;
  bool success_flag = false;
  std::default_random_engine generator;
  do {
    hash_function = GenerateHashFunction(kDefaultPrimeModule, buckets_count, generator);
    success_flag = FixedSet::CheckHashFunction(hash_function, set_numbers, buckets_count);
  } while (!success_flag);

  return hash_function;
}

class BucketData {
 public:
  BucketData() {}

  void AddElement(int element) {
    elements_.push_back(element);
  }

  const std::vector<int>& GetElements() const {
    return elements_;
  }

  void Clear() {
    elements_.clear();
  }
 private:
  std::vector<int> elements_;
};

void FixedSet::AssignBuckets(const std::vector<int> &set_numbers) {
  size_t bucket_count = set_numbers.size();
  std::vector<BucketData> buckets_elements(bucket_count);
  for (int number : set_numbers) {
    number += kMaxValue;
    int64_t index = hash_function_(number);
    buckets_elements[index].AddElement(number);
  }

  buckets_.reserve(bucket_count);
  for (int bucket_index = 0; bucket_index < bucket_count; ++bucket_index) {
    buckets_[bucket_index].Initialize(buckets_elements[bucket_index].GetElements());
    buckets_elements[bucket_index].Clear();
  }
}

void FixedSet::Initialize(const std::vector<int>& set_numbers) {
  hash_function_ = FixedSet::PickUpHashFunction(set_numbers);
  AssignBuckets(set_numbers);
}

bool FixedSet::Contains(int element) const {
  element += kMaxValue;
  int64_t index = hash_function_(element);
  return buckets_[index].Contains(element);
}
