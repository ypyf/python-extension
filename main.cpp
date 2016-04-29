#include <iostream>
#include <vector>
#include <string.h>

namespace test {
    class LogFile {
    public:
        explicit LogFile(const char* file) : file(file), row_count(0){
        }
        LogFile(const LogFile&)=delete;
        LogFile& operator=(const LogFile&)=delete;
        int rowCount() const {
            return row_count;
        }
    private:
        const char* file;
        int row_count;
    };
}

int main() {
    using namespace std;
    auto p = new test::LogFile("/data/sync_sdk_ap.log");
    cout << p->rowCount() << endl;
}
