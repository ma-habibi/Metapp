#### Build
Build by running:
> chmod +x *.sh; ./configure.sh; ./build.sh; ./run.sh

Or create a local Makefile:

```makefile
run: out
	./run.sh

build: clean
	chmod +x *.sh; ./configure.sh; ./build.sh; ./run.sh

clean:
	rm -fr out
```
