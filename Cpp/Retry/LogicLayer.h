#include <exception>
#include <string>
#include <iostream>

class LogicLayer {

public:
    LogicLayer() {

    }

    LogicLayer(NetworkLayer netLay, int retry_count) {
        this->retry_count = retry_count;
        this->netLay = netLay;
    }

    std::string read() {
        for (int i = 1; ; ++i) {
            try {
                auto str = netLay.read();
                return str;
            } catch(std::exception) {
                if (i == retry_count) {
                    throw;
                }
            }
        }
    }
private:
    NetworkLayer netLay;
    int retry_count;
};
