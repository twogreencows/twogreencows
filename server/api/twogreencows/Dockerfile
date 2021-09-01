FROM openjdk:8-alpine

COPY target/uberjar/twogreencows.jar /twogreencows/app.jar

EXPOSE 3000

CMD ["java", "-jar", "/twogreencows/app.jar"]
