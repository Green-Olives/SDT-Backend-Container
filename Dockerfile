FROM alpine:3.15 as builder
RUN apk add --no-cache \
            qt5-qtbase-dev \
            qt5-qtbase-postgresql \
            qt5-qtwebengine-dev \
            g++ \
            make 
ENV PATH=/usr/lib/qt5/bin:$PATH
ADD ./backend /src/backend
ADD ./QtWebApp /src/QtWebApp
WORKDIR /build
RUN qmake /src/backend/backend.pro && make

FROM alpine:3.15
RUN apk add --no-cache \
            qt5-qtbase \
            qt5-qtbase-postgresql \
            qt5-qtwebengine
WORKDIR /sdt
COPY --from=builder /build/backend ./backend
CMD ./backend