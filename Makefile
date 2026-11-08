CXX = g++
#Перечисляем все файлы проекта: структуры, утилиты и файлы заданий
SRCS = main3.cpp Set.cpp Stack.cpp Tree.cpp utils.cpp Array.cpp List.cpp DList.cpp Queue.cpp Task1.cpp Task2.cpp Task3.cpp Task4.cpp Task5.cpp Task6.cpp Task7.cpp
TARGET = dbms
all:
	rm -f $(TARGET)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
