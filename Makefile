.PHONY: up
up:
	docker-compose up

.PHONY: build
build:
	docker-compose build # docker-compose up --build

.PHONY: down
down:
	docker-compose down

.PHONY: logs
logs:
	docker-compose logs -f libcsv

.PHONY: debug
debug:
	docker exec -it libcsv /bin/sh
