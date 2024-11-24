FROM mysql:latest
USER root
RUN chmod 755 /var/lib/mysql
COPY ./privileges.sql /docker-entrypoint-initdb.d/
COPY ./operations.sql /docker-entrypoint-initdb.d/