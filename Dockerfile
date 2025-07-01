FROM alpine:latest as build

RUN echo "connections = 20" >> /etc/apk/apk.conf && \
    apk --no-cache add --no-cache build-base pandoc

RUN addgroup -S builder && \
    adduser -D -G builder -s /bin/sh builder && \
    chown -R builder:builder /home/builder

USER builder

WORKDIR /home/builder

COPY . .

RUN make release
RUN make site

FROM alpine:latest 

RUN addgroup -S builder && \
    adduser -D -G builder -s /bin/sh builder && \
    chown -R builder:builder /home/builder

COPY --from=build --chown=builder:builder /home/builder/site/ /home/builder/site/

USER builder

WORKDIR /home/builder
