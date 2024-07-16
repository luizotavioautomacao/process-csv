.PHONY: up
up:
	docker build -t main_image . && docker rm -f main_container && docker run -d --name main_container main_image 

.PHONY: main
main:
	./build.sh && ./main

.PHONY: test_libcsv
test_libcsv:
	./build.sh && ./test_libcsv

.PHONY: exec
exec:
	docker run --rm -d --name main_container main_image /app/main

.PHONY: logs
logs:
	docker logs main_container

.PHONY: shell
shell:
	docker exec -it main_container /bin/sh

.PHONY: del
del:
	docker container prune -f


