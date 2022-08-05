(defproject twogreencows "0.1.0-SNAPSHOT"

  :description "FIXME: write description"
  :url "http://example.com/FIXME"

  :dependencies [[ch.qos.logback/logback-classic "1.2.3"]
                 [buddy "2.0.0"]
                 [com.fasterxml.jackson.core/jackson-core "2.13.1"]
                 [cheshire "5.10.0"]
                 [clojure.java-time "0.3.2"]
                 [com.github.seancorfield/next.jdbc "1.2.659"]
                 [cljs-ajax "0.8.3"]
                 [cc.qbits/alia-all "5.0.0-alpha7"]
                 [conman "0.9.1"]
                 [cprop "0.1.17"]
                 [day8.re-frame/re-frame-10x "1.1.10"]
                 [expound "0.8.7"]
                 [http-kit "2.5.3"]
                 [funcool/struct "1.4.0"]
                 [luminus-immutant "0.2.5"]
                 [luminus-migrations "0.6.5"]
                 [luminus-transit "0.1.2"]
                 [luminus-undertow "0.1.7"]
                 [luminus/ring-ttl-session "0.3.3"]
                 [markdown-clj "1.10.5"]
                 [metosin/muuntaja "0.6.8"]
                 [metosin/reitit "0.5.15"]
                 [metosin/ring-swagger-ui "4.5.0"]
                 [metosin/ring-http-response "0.9.1"]
                 [mount "0.1.16"]
                 [nrepl "0.8.3"]
                 [org.clojure/clojure "1.10.3"]
                 [org.clojure/clojurescript "1.10.844" :scope "provided"]
                 [com.google.javascript/closure-compiler-unshaded "v20210302" :scope "provided"]
                 [org.clojure/google-closure-library "0.0-20191016-6ae1f72f" :scope "provided"]
                 [thheller/shadow-cljs "2.13.0" :scope "provided"]
                 [org.clojure/tools.cli "1.0.194"]
                 [org.clojure/tools.logging "1.1.0"]
                 [org.postgresql/postgresql "42.2.20"]
                 [org.webjars.npm/bulma "0.9.1"]
                 [org.webjars.npm/material-icons "0.3.1"]
                 [org.webjars/webjars-locator "0.40"]
                 [org.webjars/webjars-locator-jboss-vfs "0.1.0"]
                 [reagent "1.0.0"]
                 [re-frame "1.2.0"]
                 [ring-webjars "0.2.0"]
                 [ring/ring-core "1.8.2"]
                 [ring/ring-defaults "0.3.2"]
                 [selmer "1.12.31"]
                 [com.taoensso/sente "1.16.2"]]

  :min-lein-version "2.0.0"
  :source-paths ["src/clj" "src/cljs" "src/cljc" ]
  :test-paths ["test/clj"]
  :resource-paths ["resources" "target/cljsbuild"]
  :target-path "target/%s/"
  :main ^:skip-aot twogreencows.core

  :plugins [[lein-pprint "1.3.2"]]

 :clean-targets ^{:protect false} [:target-path "target/cljsbuild" ".shadow-cljs"]

  :profiles
  {:uberjar {:omit-source true
             :aot :all
             :uberjar-name "twogreencows.jar"
             :source-paths ["env/prod/clj" "env/prod/cljc" "env/prod/cljs"]
             :prep-tasks ["compile"
                          "run" "-m" "shadow.cljs.devtools.cli" "release" "app"]
             :resource-paths ["env/prod/resources"]}

   :dev           [:project/dev :profiles/dev]
   :test          [:project/dev :project/test :profiles/test]

   :project/dev  {:jvm-opts ["-Dconf=dev-config.edn"]
                  :dependencies [;[expound "0.7.2"]
                                 ;[thheller/shadow-cljs "2.13.0"]
                                 [binaryage/devtools "1.0.2"]
                                 [pjstadig/humane-test-output "0.10.0"]
                                 [prone "2020-01-17"]
                                 [ring/ring-devel "1.8.2"]
                                 [ring/ring-mock "0.4.0"]]
                  :plugins      [[com.jakemccrary/lein-test-refresh "0.24.1"]]
                  :source-paths ["env/dev/clj" "env/dev/cljs" "env/dev/cljc"]
                  :resource-paths ["env/dev/resources"]
                  :repl-options {:init-ns user
                                 :timeout 120000}
                  :injections [(require 'pjstadig.humane-test-output)
                               (pjstadig.humane-test-output/activate!)]}
   :project/test {:jvm-opts ["-Dconf=test-config.edn"]
                  :resource-paths ["env/test/resources"]}
   :profiles/dev {}
   :profiles/test {}})
