from alpine:latest

RUN apk add --no-cache build-base pandoc

WORKDIR /workdir

COPY . .

RUN mkdir sites
RUN mkdir pages
RUN gcc site_generator.c -o generate



