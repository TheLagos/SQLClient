cmake_minimum_required(VERSION 3.14)
project(MySQLClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(MYSQL_SERVER_PATH "C:/Program Files/MySQL/MySQL Server 8.0")

find_path(MYSQL_INCLUDE_DIR mysql.h
    HINTS "${MYSQL_SERVER_PATH}/include"
    DOC "MySQL include directory"
)

find_library(MYSQL_LIBRARY libmysql
    HINTS "${MYSQL_SERVER_PATH}/lib"
    DOC "MySQL library file"
)

if(NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_LIBRARY)
    message(FATAL_ERROR "ПОМИЛКА: Не вдалося знайти mysql.h або libmysql.lib.\n"
                        "Перевір шлях у змінній MYSQL_SERVER_PATH у CMakeLists.txt.\n"
                        "Поточний (неправильний) шлях: ${MYSQL_SERVER_PATH}")
else()
    message(STATUS "Знайдено MySQL include: ${MYSQL_INCLUDE_DIR}")
    message(STATUS "Знайдено MySQL library: ${MYSQL_LIBRARY}")
endif()

add_executable(my_program main.cpp "sql_client.h" "sql_client.cpp" "user.h" "user_dao.cpp" "user_record.cpp")

target_include_directories(my_program
    PRIVATE
    ${MYSQL_INCLUDE_DIR}
)

target_link_libraries(my_program
    PRIVATE
    ${MYSQL_LIBRARY}
)