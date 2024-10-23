CC=gcc
CFLAGS= -Wall -Werror -Wextra -std=c11
BRK = brick_game
GAME = tetris
GCL = gui/cli
BACK = $(BRK)/$(GAME)/backend.c
BACK_OBJ = $(patsubst %.c,%.o,$(BACK))
FSM_FRONT_MAIN = $(BRK)/$(GAME)/fsm.c $(BRK)/$(GAME)/tetris.c $(GCL)/frontend.c
FSM_FRONT_MAIN_OBJ = $(patsubst %.c,%.o,$(FSM_FRONT_MAIN))
NCURS = -lncurses
COV = -coverage
TEST_DIR = tests
TEST = $(TEST_DIR)/tests.c
BUILD = build
DIST = dist
DOC = documentation
OS = $(shell uname)

ifeq ($(OS),Linux)
  det_OS = -lcheck -lm -lrt -lpthread -lsubunit -lncurses
else
  det_OS = -lcheck -lm -lpthread -lncurses
endif

all: clean_all gcov_report dist

$(BACK_OBJ): $(BACK)
	$(CC) $(CFLAGS) -c $< -o $@ $(det_OS)

$(GAME).a: $(BACK_OBJ)
	ar rcs $(GAME).a $<
	ranlib $(GAME).a

$(FSM_FRONT_MAIN_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(GAME): $(FSM_FRONT_MAIN_OBJ) $(GAME).a 
	$(CC) $(CFLAGS) $(FSM_FRONT_MAIN_OBJ) $(GAME).a $(NCURS) -o $@ $(det_OS)

install:
	make $(GAME) && mkdir $(BUILD) && mv ./$(GAME) $(BUILD)

play:
	./$(BUILD)/$(GAME)

uninstall:
	rm -rf ./$(BUILD)

test: 
	$(CC) $(CFLAGS) $(COV) $(BACK) $(TEST) -o $(TEST_DIR)/$@ $(det_OS)
	./$(TEST_DIR)/$@

gcov_report: test
	./$(TEST_DIR)/$<
	lcov -o ./$(TEST_DIR)/res.info -c -d .
	genhtml -o ./$(TEST_DIR)/report ./$(TEST_DIR)/res.info
	# open ./$(TEST_DIR)/report/index.html

install_doxygen:
	sudo apt install graphviz && sudo apt install doxygen

dvi: 
	doxygen
	# google-chrome documentation/html/index.html

dist: install dvi
	mkdir -p $(DIST)
	cp -R ./$(BUILD)/$(GAME) ./$(DIST)/$(GAME)
	cp -R ./$(DOC) ./$(DIST)/$(DOC)
	cd ./$(DIST) && tar cvzf $(GAME).tgz *
	cd ./$(DIST) && rm -rf $(GAME) $(DOC)
	make clean

check:
	cp ../materials/linters/.clang-format ./$(BRK)/$(GAME)
	clang-format -style=Google -n ./$(BRK)/$(GAME)/*.c ./$(BRK)/$(GAME)/*.h
	clang-format -style=Google -i ./$(BRK)/$(GAME)/*.c ./$(BRK)/$(GAME)/*.h
	rm ./$(BRK)/$(GAME)/.clang-format
	cp ../materials/linters/.clang-format ./gui/cli
	clang-format -style=Google -n ./$(GCL)/*.c ./$(GCL)/*.h
	clang-format -style=Google -i ./$(GCL)/*.c ./$(GCL)/*.h
	rm ./$(GCL)/.clang-format

valgrind: test
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -s ./$(TEST_DIR)/$<

clean:
	rm -rf ./comp ./$(TEST_DIR)/report ./$(TEST_DIR)/*.gc* ./$(TEST_DIR)/test *.o ./**/**/*.o ./$(TEST_DIR)/*.info *.a ./$(TEST_DIR)/test.dSYM test ./$(BUILD) ./$(DOC) ./high_score.txt

clean_dist:
	rm -rf dist

clean_all: clean clean_dist

rebuild: clean all
