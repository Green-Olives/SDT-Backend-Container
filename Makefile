DOCKER_LOGIN=5bondarenko
DOCKER_TOKEN=5144f848-5031-4ffa-bde0-435fb5db8172
CONTAINER_NAME=sdt-course-work-backend
CONTAINER_TAG=latest

all: build

build:
	echo ${DOCKER_TOKEN} | docker login -u ${DOCKER_LOGIN} --password-stdin
	docker build -t ${DOCKER_LOGIN}/${CONTAINER_NAME}:${CONTAINER_TAG} .
	docker push ${DOCKER_LOGIN}/${CONTAINER_NAME}:${CONTAINER_TAG}