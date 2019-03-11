#include <iostream>
#include <pqxx/pqxx> 


using namespace pqxx;

int main(int argc, char* argv[]) {
  try {
    pqxx::connection C("dbname = OnlineData user = postgres password = Sheffield2016 hostaddr = 127.0.0.1 port = 5432");
    if (C.is_open()) {
      std::cout << "Opened database successfully: " << C.dbname() << std::endl;
    } else {
      std::cout << "Can't open database" << std::endl;
      return 1;
    }
    C.disconnect ();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
