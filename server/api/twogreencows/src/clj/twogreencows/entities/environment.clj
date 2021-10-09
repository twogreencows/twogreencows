(ns twogreencoes.entities.environment
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
  ))

(def environment-object-version 1)
(def environment-prefix "env")
