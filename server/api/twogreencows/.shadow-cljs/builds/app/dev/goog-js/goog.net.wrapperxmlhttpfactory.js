["^ ","~:resource-id",["~:shadow.build.classpath/resource","goog/net/wrapperxmlhttpfactory.js"],"~:js","goog.provide(\"goog.net.WrapperXmlHttpFactory\");\ngoog.require(\"goog.net.XhrLike\");\ngoog.require(\"goog.net.XmlHttpFactory\");\ngoog.net.WrapperXmlHttpFactory = function(xhrFactory, optionsFactory) {\n  goog.net.XmlHttpFactory.call(this);\n  this.xhrFactory_ = xhrFactory;\n  this.optionsFactory_ = optionsFactory;\n};\ngoog.inherits(goog.net.WrapperXmlHttpFactory, goog.net.XmlHttpFactory);\ngoog.net.WrapperXmlHttpFactory.prototype.createInstance = function() {\n  return this.xhrFactory_();\n};\ngoog.net.WrapperXmlHttpFactory.prototype.getOptions = function() {\n  return this.optionsFactory_();\n};\n","~:source","// Copyright 2010 The Closure Library Authors. All Rights Reserved.\n//\n// Licensed under the Apache License, Version 2.0 (the \"License\");\n// you may not use this file except in compliance with the License.\n// You may obtain a copy of the License at\n//\n//      http://www.apache.org/licenses/LICENSE-2.0\n//\n// Unless required by applicable law or agreed to in writing, software\n// distributed under the License is distributed on an \"AS-IS\" BASIS,\n// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n// See the License for the specific language governing permissions and\n// limitations under the License.\n\n/**\n * @fileoverview Implementation of XmlHttpFactory which allows construction from\n * simple factory methods.\n * @author dbk@google.com (David Barrett-Kahn)\n */\n\ngoog.provide('goog.net.WrapperXmlHttpFactory');\n\n/** @suppress {extraRequire} Typedef. */\ngoog.require('goog.net.XhrLike');\ngoog.require('goog.net.XmlHttpFactory');\n\n\n\n/**\n * An xhr factory subclass which can be constructed using two factory methods.\n * This exists partly to allow the preservation of goog.net.XmlHttp.setFactory()\n * with an unchanged signature.\n * @param {function():!goog.net.XhrLike.OrNative} xhrFactory\n *     A function which returns a new XHR object.\n * @param {function():!Object} optionsFactory A function which returns the\n *     options associated with xhr objects from this factory.\n * @extends {goog.net.XmlHttpFactory}\n * @constructor\n * @final\n */\ngoog.net.WrapperXmlHttpFactory = function(xhrFactory, optionsFactory) {\n  goog.net.XmlHttpFactory.call(this);\n\n  /**\n   * XHR factory method.\n   * @type {function() : !goog.net.XhrLike.OrNative}\n   * @private\n   */\n  this.xhrFactory_ = xhrFactory;\n\n  /**\n   * Options factory method.\n   * @type {function() : !Object}\n   * @private\n   */\n  this.optionsFactory_ = optionsFactory;\n};\ngoog.inherits(goog.net.WrapperXmlHttpFactory, goog.net.XmlHttpFactory);\n\n\n/** @override */\ngoog.net.WrapperXmlHttpFactory.prototype.createInstance = function() {\n  return this.xhrFactory_();\n};\n\n\n/** @override */\ngoog.net.WrapperXmlHttpFactory.prototype.getOptions = function() {\n  return this.optionsFactory_();\n};\n","~:compiled-at",1626505468017,"~:source-map-json","{\n\"version\":3,\n\"file\":\"goog.net.wrapperxmlhttpfactory.js\",\n\"lineCount\":16,\n\"mappings\":\"AAoBAA,IAAKC,CAAAA,OAAL,CAAa,gCAAb,CAAA;AAGAD,IAAKE,CAAAA,OAAL,CAAa,kBAAb,CAAA;AACAF,IAAKE,CAAAA,OAAL,CAAa,yBAAb,CAAA;AAgBAF,IAAKG,CAAAA,GAAIC,CAAAA,qBAAT,GAAiCC,QAAQ,CAACC,UAAD,EAAaC,cAAb,CAA6B;AACpEP,MAAKG,CAAAA,GAAIK,CAAAA,cAAeC,CAAAA,IAAxB,CAA6B,IAA7B,CAAA;AAOA,MAAKC,CAAAA,WAAL,GAAmBJ,UAAnB;AAOA,MAAKK,CAAAA,eAAL,GAAuBJ,cAAvB;AAfoE,CAAtE;AAiBAP,IAAKY,CAAAA,QAAL,CAAcZ,IAAKG,CAAAA,GAAIC,CAAAA,qBAAvB,EAA8CJ,IAAKG,CAAAA,GAAIK,CAAAA,cAAvD,CAAA;AAIAR,IAAKG,CAAAA,GAAIC,CAAAA,qBAAsBS,CAAAA,SAAUC,CAAAA,cAAzC,GAA0DC,QAAQ,EAAG;AACnE,SAAO,IAAKL,CAAAA,WAAL,EAAP;AADmE,CAArE;AAMAV,IAAKG,CAAAA,GAAIC,CAAAA,qBAAsBS,CAAAA,SAAUG,CAAAA,UAAzC,GAAsDC,QAAQ,EAAG;AAC/D,SAAO,IAAKN,CAAAA,eAAL,EAAP;AAD+D,CAAjE;;\",\n\"sources\":[\"goog/net/wrapperxmlhttpfactory.js\"],\n\"sourcesContent\":[\"// Copyright 2010 The Closure Library Authors. All Rights Reserved.\\n//\\n// Licensed under the Apache License, Version 2.0 (the \\\"License\\\");\\n// you may not use this file except in compliance with the License.\\n// You may obtain a copy of the License at\\n//\\n//      http://www.apache.org/licenses/LICENSE-2.0\\n//\\n// Unless required by applicable law or agreed to in writing, software\\n// distributed under the License is distributed on an \\\"AS-IS\\\" BASIS,\\n// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\\n// See the License for the specific language governing permissions and\\n// limitations under the License.\\n\\n/**\\n * @fileoverview Implementation of XmlHttpFactory which allows construction from\\n * simple factory methods.\\n * @author dbk@google.com (David Barrett-Kahn)\\n */\\n\\ngoog.provide('goog.net.WrapperXmlHttpFactory');\\n\\n/** @suppress {extraRequire} Typedef. */\\ngoog.require('goog.net.XhrLike');\\ngoog.require('goog.net.XmlHttpFactory');\\n\\n\\n\\n/**\\n * An xhr factory subclass which can be constructed using two factory methods.\\n * This exists partly to allow the preservation of goog.net.XmlHttp.setFactory()\\n * with an unchanged signature.\\n * @param {function():!goog.net.XhrLike.OrNative} xhrFactory\\n *     A function which returns a new XHR object.\\n * @param {function():!Object} optionsFactory A function which returns the\\n *     options associated with xhr objects from this factory.\\n * @extends {goog.net.XmlHttpFactory}\\n * @constructor\\n * @final\\n */\\ngoog.net.WrapperXmlHttpFactory = function(xhrFactory, optionsFactory) {\\n  goog.net.XmlHttpFactory.call(this);\\n\\n  /**\\n   * XHR factory method.\\n   * @type {function() : !goog.net.XhrLike.OrNative}\\n   * @private\\n   */\\n  this.xhrFactory_ = xhrFactory;\\n\\n  /**\\n   * Options factory method.\\n   * @type {function() : !Object}\\n   * @private\\n   */\\n  this.optionsFactory_ = optionsFactory;\\n};\\ngoog.inherits(goog.net.WrapperXmlHttpFactory, goog.net.XmlHttpFactory);\\n\\n\\n/** @override */\\ngoog.net.WrapperXmlHttpFactory.prototype.createInstance = function() {\\n  return this.xhrFactory_();\\n};\\n\\n\\n/** @override */\\ngoog.net.WrapperXmlHttpFactory.prototype.getOptions = function() {\\n  return this.optionsFactory_();\\n};\\n\"],\n\"names\":[\"goog\",\"provide\",\"require\",\"net\",\"WrapperXmlHttpFactory\",\"goog.net.WrapperXmlHttpFactory\",\"xhrFactory\",\"optionsFactory\",\"XmlHttpFactory\",\"call\",\"xhrFactory_\",\"optionsFactory_\",\"inherits\",\"prototype\",\"createInstance\",\"goog.net.WrapperXmlHttpFactory.prototype.createInstance\",\"getOptions\",\"goog.net.WrapperXmlHttpFactory.prototype.getOptions\"]\n}\n"]