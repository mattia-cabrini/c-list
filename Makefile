CC="gcc"
DD="lldb"
LIBS="c-test/x/test.a"

list: prepare
	$(CC) list.c -c -o x/list.a

test: list test_list_int
	$(CC) x/list.a x/test_list_int.a $(LIBS) -o x/test
	x/test
	rm x/test

test_list_int:
	$(CC) test_list_int.c -c -o x/test_list_int.a

clean:
	[ ! -d x ] || rm -r x

prepare:
	[ -d x ] || mkdir x
