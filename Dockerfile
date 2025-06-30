from alpine:latest

RUN apk add --no-cache build-base pandoc

WORKDIR /workdir

COPY . .

RUN make release
RUN make site
