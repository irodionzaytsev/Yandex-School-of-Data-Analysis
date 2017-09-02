#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <algorithm>
#include <list>
#include <ctime>
#include <complex>

template <typename T>
void printPointers(const std::vector<T*>& some_vector) {
    for (T* element : some_vector) {
        std::cerr << *element << "\n";
    }
}

template <typename T>
void printVector(const std::vector<T>& some_vector) {
    for (T element : some_vector) {
        std::cerr << element << " ";
    }
    std::cerr << "\n";
}

void writeOutput(int answer,
                 std::ostream &out_stream = std::cout) {
    out_stream << answer << "\n";
}

// fft is taken from e-maxx algo
const long double PI = 3.1415926;
typedef std::complex<double> base;

int rev (int num, int lg_n) {
    int res = 0;
    for (int i = 0; i < lg_n; ++i) {
        if (num & (1 << i)) {
            res |= 1 << (lg_n - 1 - i);
        }
    }
    return res;
}

void fft (std::vector<base>& set_a, bool invert) {
    int size_a = set_a.size();
    int lg_n = 0;
    while ((1 << lg_n) < size_a) {
        ++lg_n;
    }

    for (int i = 0; i < size_a; ++i) {
        if (i < rev(i, lg_n)) {
            std::swap(set_a[i], set_a[rev(i, lg_n)]);
        }
    }

    for (int len = 2; len <= size_a; len <<= 1) {
        double ang = 2*PI/len * (invert ? -1 : 1);
        base wlen (cos(ang), sin(ang));
        for (int i = 0; i < size_a; i += len) {
            base w(1);
            for (int j = 0; j < len / 2; ++j) {
                base u = set_a[i + j];
                base v = set_a[i + j + len / 2] * w;
                set_a[i + j] = u + v;
                set_a[i + j + len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < size_a; ++i) {
            set_a[i] /= size_a;
        }
    }
}

std::vector<int32_t> multiply (std::vector<int8_t>& set_a,
                               std::vector<int8_t>& set_b) {
    std::vector<base> fourie_a (set_a.begin(), set_a.end());
    set_a.clear();
    std::vector<base> fourie_b (set_b.begin(), set_b.end());
    set_b.clear();
    size_t degree = 1;
    while (degree < std::max (fourie_a.size(), fourie_b.size())) {
        degree <<= 1;
    }

    fourie_a.resize (degree);
    fourie_b.resize (degree);

    fft (fourie_a, false);
    fft (fourie_b, false);

    for (size_t i = 0; i < degree; ++i) {
        fourie_a[i] *= fourie_b[i];
    }
    fourie_b.clear();
    fft (fourie_a, true);

    std::reverse(fourie_a.begin(), fourie_a.end());

    std::vector<int32_t> res;

    while (fourie_a.size() > 0) {
        res.push_back(int32_t(fourie_a.back().real() + 0.5));
        fourie_a.pop_back();
    }

    return res;
}

std::vector<int32_t> cyclics(std::vector<int8_t>& set_a, std::vector<int8_t>& set_b) {
    std::vector<int8_t> copy_a;
    for (int i = 0; i < set_a.size(); ++i) {
        int govno_ebanoje_v_rot_ebal_ssanye_casty = set_a.size();
        copy_a.push_back(set_a[govno_ebanoje_v_rot_ebal_ssanye_casty - i - 1]);
    }
    // for (int i = 0; i < set_a.size(); ++i) {
    //     copy_a.push_back(0);
    // }

    set_a.clear();

    std::vector<int8_t> copy_b;
    for (int i = 0; i < set_b.size(); ++i) {
        copy_b.push_back(set_b[i]);
    }
    // for (int i = set_b.size(); i < copy_a.size(); ++i) {
    //     copy_b.push_back(0);
    // }
    // for (int i = 0; i < set_b.size(); ++i) {
    //     copy_b.push_back(set_b[i]);
    // }
    // for (int i = set_b.size(); i < copy_a.size() / 2; ++i) {
    //     copy_b.push_back(0);
    // }

    set_b.clear();

    std::vector<int32_t> mult = multiply(copy_a, copy_b);
    return mult;
}

std::string readDNA(int len = 0) {
    std::string dna;
    if (len > 0) {
        for (int in = 0; in < len; ++in) {
            int ran = rand() % 4;
            if (ran == 0) {
                dna.push_back('A');
            }
            if (ran == 1) {
                dna.push_back('C');
            }
            if (ran == 2) {
                dna.push_back('G');
            }
            if (ran == 3) {
                dna.push_back('T');
            }
        }
        return dna;
    }
    std::cin >> dna;
    std::string char_dna;
    for (char character : dna) {
        if (character == 'A') {
            char_dna += "0001";
        }
        if (character == 'C') {
            char_dna += "0010";
        }
        if (character == 'G') {
            char_dna += "0100";
        }
        if (character == 'T') {
            char_dna += "1000";
        }
    }
    return char_dna;
}

std::vector<int8_t> cast_dna(const std::string& dna) {
    std::vector<int8_t> casted;
    for (int i = 0; i < dna.size(); ++i) {
        casted.push_back(dna[i] - '0');
    }
    return casted;
}

int solve(std::vector<int8_t>& first_vector_dna, std::vector<int8_t>& second_vector_dna) {
    int first_comp_size = first_vector_dna.size();
    int second_comp_size = second_vector_dna.size();
    // printVector(first_vector_dna);
    // printVector(second_vector_dna);
    std::vector<int32_t> mult_dna = cyclics(first_vector_dna, second_vector_dna);
    // for (int i = 0; i < mult_dna.size(); ++i) {
    //     std::cout << mult_dna[i] << " ";
    // }
    // std::cout << "\n";
    int32_t maximal = -1;

    for (int t = 0; t <= (first_comp_size - second_comp_size) / 4; ++t) {
        maximal = std::max(maximal, mult_dna[first_comp_size - 4 * t - 1]);
    }

    // std::cout << "mx = " << maximal << "\n";

    for (int t = 0; t <= (first_comp_size - second_comp_size) / 4; ++t) {
        if (maximal == mult_dna[first_comp_size - 4 * t - 1]) {
            return t + 1;
        }
    }

    int shad_obossanoje_govno_vmeste_so_svoim_pcfom = 0;
    return shad_obossanoje_govno_vmeste_so_svoim_pcfom;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::vector<int8_t> first_vector_dna = cast_dna(readDNA());
    std::vector<int8_t> second_vector_dna = cast_dna(readDNA());
    int answer = solve(first_vector_dna, second_vector_dna);
    std::cout << answer << "\n";
}
