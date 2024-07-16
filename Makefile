.PHONY: up
up:
	docker build -t main_image . && docker rm -f main_container && docker run -d --name main_container main_image 

.PHONY: main
main:
	./build.sh && ./main

.PHONY: test_libcsv
test_libcsv:
	./build.sh && chmod +x ./test_libcsv && ./test_libcsv

.PHONY: exec_main
exec_main:
	docker run --rm -d --name main_container main_image /app/main

.PHONY: exec_test_libcsv
exec_test_libcsv:
	docker run --rm -d --name main_container main_image /app/test_libcsv

.PHONY: logs
logs:
	docker logs main_container

.PHONY: shell
shell:
	docker exec -it main_container /bin/sh

.PHONY: entrypoint
entrypoint:
	docker run -it --rm --entrypoint /bin/sh main_image

.PHONY: del
del:
	docker container prune -f


