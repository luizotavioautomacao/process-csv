Executar docker: `make up`  
Debugar .so com main.c:  
```
cd src
make up
```  
  
Run compiled main file: `make host`

# Comandos docker:
- docker images  
- docker ps -a  
- docker build && docker run  
- docker run -it container_id /bin/sh  
- docker start -ai container_id
- docker images
# Executar no host:
- chmod +x test_libcsv  
- ./test_libcsv  
# Git push:
export LANG=C.UTF-8  
export LC_ALL=C.UTF-8  