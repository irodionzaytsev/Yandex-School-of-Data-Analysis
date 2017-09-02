//
//  retry.h
//  LAYER_RETRY
//
//  Created by Shabanov Lev on 16/11/2016.
//  Copyright © 2016 Shabanov Lev. All rights reserved.
//

#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

class NetworkLayer {
public:
    std::string read() {
        int coef = rand() % 5;
        std::cerr << coef << std::endl;
        if (coef == 0) {
            throw std::exception();
        } else {
            std::cerr << "putin";
            return "kek";
        }
    }
};

class LogicLayer {
public:
    LogicLayer(NetworkLayer object, int tries) {
        network = object;
        tries_ = tries;
    }
    std::string read() {
        std::cerr << tries_ << '\n';
        for (int i = 0;; ++i) {
            try {
                std::cerr << "read\n";
                auto answer = network.read();
                std::cerr << "llo " << answer << '\n';
                return answer;
            } catch (...) {
                std::cerr << "exception\n";
                continue;
                if (i == tries_) {
                    throw;
                }
            }
        }
    }
private:
    NetworkLayer network;
    int tries_;
};
