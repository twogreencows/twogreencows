["^ ","~:resource-id",["~:shadow.build.classpath/resource","goog/math/coordinate.js"],"~:js","goog.provide(\"goog.math.Coordinate\");\ngoog.require(\"goog.math\");\ngoog.math.Coordinate = function(opt_x, opt_y) {\n  this.x = opt_x !== undefined ? opt_x : 0;\n  this.y = opt_y !== undefined ? opt_y : 0;\n};\ngoog.math.Coordinate.prototype.clone = function() {\n  return new goog.math.Coordinate(this.x, this.y);\n};\nif (goog.DEBUG) {\n  goog.math.Coordinate.prototype.toString = function() {\n    return \"(\" + this.x + \", \" + this.y + \")\";\n  };\n}\ngoog.math.Coordinate.prototype.equals = function(other) {\n  return other instanceof goog.math.Coordinate && goog.math.Coordinate.equals(this, other);\n};\ngoog.math.Coordinate.equals = function(a, b) {\n  if (a == b) {\n    return true;\n  }\n  if (!a || !b) {\n    return false;\n  }\n  return a.x == b.x && a.y == b.y;\n};\ngoog.math.Coordinate.distance = function(a, b) {\n  var dx = a.x - b.x;\n  var dy = a.y - b.y;\n  return Math.sqrt(dx * dx + dy * dy);\n};\ngoog.math.Coordinate.magnitude = function(a) {\n  return Math.sqrt(a.x * a.x + a.y * a.y);\n};\ngoog.math.Coordinate.azimuth = function(a) {\n  return goog.math.angle(0, 0, a.x, a.y);\n};\ngoog.math.Coordinate.squaredDistance = function(a, b) {\n  var dx = a.x - b.x;\n  var dy = a.y - b.y;\n  return dx * dx + dy * dy;\n};\ngoog.math.Coordinate.difference = function(a, b) {\n  return new goog.math.Coordinate(a.x - b.x, a.y - b.y);\n};\ngoog.math.Coordinate.sum = function(a, b) {\n  return new goog.math.Coordinate(a.x + b.x, a.y + b.y);\n};\ngoog.math.Coordinate.prototype.ceil = function() {\n  this.x = Math.ceil(this.x);\n  this.y = Math.ceil(this.y);\n  return this;\n};\ngoog.math.Coordinate.prototype.floor = function() {\n  this.x = Math.floor(this.x);\n  this.y = Math.floor(this.y);\n  return this;\n};\ngoog.math.Coordinate.prototype.round = function() {\n  this.x = Math.round(this.x);\n  this.y = Math.round(this.y);\n  return this;\n};\ngoog.math.Coordinate.prototype.translate = function(tx, opt_ty) {\n  if (tx instanceof goog.math.Coordinate) {\n    this.x += tx.x;\n    this.y += tx.y;\n  } else {\n    this.x += Number(tx);\n    if (typeof opt_ty === \"number\") {\n      this.y += opt_ty;\n    }\n  }\n  return this;\n};\ngoog.math.Coordinate.prototype.scale = function(sx, opt_sy) {\n  var sy = typeof opt_sy === \"number\" ? opt_sy : sx;\n  this.x *= sx;\n  this.y *= sy;\n  return this;\n};\ngoog.math.Coordinate.prototype.rotateRadians = function(radians, opt_center) {\n  var center = opt_center || new goog.math.Coordinate(0, 0);\n  var x = this.x;\n  var y = this.y;\n  var cos = Math.cos(radians);\n  var sin = Math.sin(radians);\n  this.x = (x - center.x) * cos - (y - center.y) * sin + center.x;\n  this.y = (x - center.x) * sin + (y - center.y) * cos + center.y;\n};\ngoog.math.Coordinate.prototype.rotateDegrees = function(degrees, opt_center) {\n  this.rotateRadians(goog.math.toRadians(degrees), opt_center);\n};\n","~:source","// Copyright 2006 The Closure Library Authors. All Rights Reserved.\n//\n// Licensed under the Apache License, Version 2.0 (the \"License\");\n// you may not use this file except in compliance with the License.\n// You may obtain a copy of the License at\n//\n//      http://www.apache.org/licenses/LICENSE-2.0\n//\n// Unless required by applicable law or agreed to in writing, software\n// distributed under the License is distributed on an \"AS-IS\" BASIS,\n// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n// See the License for the specific language governing permissions and\n// limitations under the License.\n\n/**\n * @fileoverview A utility class for representing two-dimensional positions.\n */\n\n\ngoog.provide('goog.math.Coordinate');\n\ngoog.require('goog.math');\n\n\n\n/**\n * Class for representing coordinates and positions.\n * @param {number=} opt_x Left, defaults to 0.\n * @param {number=} opt_y Top, defaults to 0.\n * @struct\n * @constructor\n */\ngoog.math.Coordinate = function(opt_x, opt_y) {\n  /**\n   * X-value\n   * @type {number}\n   */\n  this.x = (opt_x !== undefined) ? opt_x : 0;\n\n  /**\n   * Y-value\n   * @type {number}\n   */\n  this.y = (opt_y !== undefined) ? opt_y : 0;\n};\n\n\n/**\n * Returns a new copy of the coordinate.\n * @return {!goog.math.Coordinate} A clone of this coordinate.\n */\ngoog.math.Coordinate.prototype.clone = function() {\n  return new goog.math.Coordinate(this.x, this.y);\n};\n\n\nif (goog.DEBUG) {\n  /**\n   * Returns a nice string representing the coordinate.\n   * @return {string} In the form (50, 73).\n   * @override\n   */\n  goog.math.Coordinate.prototype.toString = function() {\n    return '(' + this.x + ', ' + this.y + ')';\n  };\n}\n\n\n/**\n * Returns whether the specified value is equal to this coordinate.\n * @param {*} other Some other value.\n * @return {boolean} Whether the specified value is equal to this coordinate.\n */\ngoog.math.Coordinate.prototype.equals = function(other) {\n  return other instanceof goog.math.Coordinate &&\n      goog.math.Coordinate.equals(this, other);\n};\n\n\n/**\n * Compares coordinates for equality.\n * @param {goog.math.Coordinate} a A Coordinate.\n * @param {goog.math.Coordinate} b A Coordinate.\n * @return {boolean} True iff the coordinates are equal, or if both are null.\n */\ngoog.math.Coordinate.equals = function(a, b) {\n  if (a == b) {\n    return true;\n  }\n  if (!a || !b) {\n    return false;\n  }\n  return a.x == b.x && a.y == b.y;\n};\n\n\n/**\n * Returns the distance between two coordinates.\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @param {!goog.math.Coordinate} b A Coordinate.\n * @return {number} The distance between `a` and `b`.\n */\ngoog.math.Coordinate.distance = function(a, b) {\n  var dx = a.x - b.x;\n  var dy = a.y - b.y;\n  return Math.sqrt(dx * dx + dy * dy);\n};\n\n\n/**\n * Returns the magnitude of a coordinate.\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @return {number} The distance between the origin and `a`.\n */\ngoog.math.Coordinate.magnitude = function(a) {\n  return Math.sqrt(a.x * a.x + a.y * a.y);\n};\n\n\n/**\n * Returns the angle from the origin to a coordinate.\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @return {number} The angle, in degrees, clockwise from the positive X\n *     axis to `a`.\n */\ngoog.math.Coordinate.azimuth = function(a) {\n  return goog.math.angle(0, 0, a.x, a.y);\n};\n\n\n/**\n * Returns the squared distance between two coordinates. Squared distances can\n * be used for comparisons when the actual value is not required.\n *\n * Performance note: eliminating the square root is an optimization often used\n * in lower-level languages, but the speed difference is not nearly as\n * pronounced in JavaScript (only a few percent.)\n *\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @param {!goog.math.Coordinate} b A Coordinate.\n * @return {number} The squared distance between `a` and `b`.\n */\ngoog.math.Coordinate.squaredDistance = function(a, b) {\n  var dx = a.x - b.x;\n  var dy = a.y - b.y;\n  return dx * dx + dy * dy;\n};\n\n\n/**\n * Returns the difference between two coordinates as a new\n * goog.math.Coordinate.\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @param {!goog.math.Coordinate} b A Coordinate.\n * @return {!goog.math.Coordinate} A Coordinate representing the difference\n *     between `a` and `b`.\n */\ngoog.math.Coordinate.difference = function(a, b) {\n  return new goog.math.Coordinate(a.x - b.x, a.y - b.y);\n};\n\n\n/**\n * Returns the sum of two coordinates as a new goog.math.Coordinate.\n * @param {!goog.math.Coordinate} a A Coordinate.\n * @param {!goog.math.Coordinate} b A Coordinate.\n * @return {!goog.math.Coordinate} A Coordinate representing the sum of the two\n *     coordinates.\n */\ngoog.math.Coordinate.sum = function(a, b) {\n  return new goog.math.Coordinate(a.x + b.x, a.y + b.y);\n};\n\n\n/**\n * Rounds the x and y fields to the next larger integer values.\n * @return {!goog.math.Coordinate} This coordinate with ceil'd fields.\n */\ngoog.math.Coordinate.prototype.ceil = function() {\n  this.x = Math.ceil(this.x);\n  this.y = Math.ceil(this.y);\n  return this;\n};\n\n\n/**\n * Rounds the x and y fields to the next smaller integer values.\n * @return {!goog.math.Coordinate} This coordinate with floored fields.\n */\ngoog.math.Coordinate.prototype.floor = function() {\n  this.x = Math.floor(this.x);\n  this.y = Math.floor(this.y);\n  return this;\n};\n\n\n/**\n * Rounds the x and y fields to the nearest integer values.\n * @return {!goog.math.Coordinate} This coordinate with rounded fields.\n */\ngoog.math.Coordinate.prototype.round = function() {\n  this.x = Math.round(this.x);\n  this.y = Math.round(this.y);\n  return this;\n};\n\n\n/**\n * Translates this box by the given offsets. If a `goog.math.Coordinate`\n * is given, then the x and y values are translated by the coordinate's x and y.\n * Otherwise, x and y are translated by `tx` and `opt_ty`\n * respectively.\n * @param {number|goog.math.Coordinate} tx The value to translate x by or the\n *     the coordinate to translate this coordinate by.\n * @param {number=} opt_ty The value to translate y by.\n * @return {!goog.math.Coordinate} This coordinate after translating.\n */\ngoog.math.Coordinate.prototype.translate = function(tx, opt_ty) {\n  if (tx instanceof goog.math.Coordinate) {\n    this.x += tx.x;\n    this.y += tx.y;\n  } else {\n    this.x += Number(tx);\n    if (typeof opt_ty === 'number') {\n      this.y += opt_ty;\n    }\n  }\n  return this;\n};\n\n\n/**\n * Scales this coordinate by the given scale factors. The x and y values are\n * scaled by `sx` and `opt_sy` respectively.  If `opt_sy`\n * is not given, then `sx` is used for both x and y.\n * @param {number} sx The scale factor to use for the x dimension.\n * @param {number=} opt_sy The scale factor to use for the y dimension.\n * @return {!goog.math.Coordinate} This coordinate after scaling.\n */\ngoog.math.Coordinate.prototype.scale = function(sx, opt_sy) {\n  var sy = (typeof opt_sy === 'number') ? opt_sy : sx;\n  this.x *= sx;\n  this.y *= sy;\n  return this;\n};\n\n\n/**\n * Rotates this coordinate clockwise about the origin (or, optionally, the given\n * center) by the given angle, in radians.\n * @param {number} radians The angle by which to rotate this coordinate\n *     clockwise about the given center, in radians.\n * @param {!goog.math.Coordinate=} opt_center The center of rotation. Defaults\n *     to (0, 0) if not given.\n */\ngoog.math.Coordinate.prototype.rotateRadians = function(radians, opt_center) {\n  var center = opt_center || new goog.math.Coordinate(0, 0);\n\n  var x = this.x;\n  var y = this.y;\n  var cos = Math.cos(radians);\n  var sin = Math.sin(radians);\n\n  this.x = (x - center.x) * cos - (y - center.y) * sin + center.x;\n  this.y = (x - center.x) * sin + (y - center.y) * cos + center.y;\n};\n\n\n/**\n * Rotates this coordinate clockwise about the origin (or, optionally, the given\n * center) by the given angle, in degrees.\n * @param {number} degrees The angle by which to rotate this coordinate\n *     clockwise about the given center, in degrees.\n * @param {!goog.math.Coordinate=} opt_center The center of rotation. Defaults\n *     to (0, 0) if not given.\n */\ngoog.math.Coordinate.prototype.rotateDegrees = function(degrees, opt_center) {\n  this.rotateRadians(goog.math.toRadians(degrees), opt_center);\n};\n","~:compiled-at",1626505467044,"~:source-map-json","{\n\"version\":3,\n\"file\":\"goog.math.coordinate.js\",\n\"lineCount\":94,\n\"mappings\":\"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\",\n\"sources\":[\"goog/math/coordinate.js\"],\n\"sourcesContent\":[\"// Copyright 2006 The Closure Library Authors. All Rights Reserved.\\n//\\n// Licensed under the Apache License, Version 2.0 (the \\\"License\\\");\\n// you may not use this file except in compliance with the License.\\n// You may obtain a copy of the License at\\n//\\n//      http://www.apache.org/licenses/LICENSE-2.0\\n//\\n// Unless required by applicable law or agreed to in writing, software\\n// distributed under the License is distributed on an \\\"AS-IS\\\" BASIS,\\n// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\\n// See the License for the specific language governing permissions and\\n// limitations under the License.\\n\\n/**\\n * @fileoverview A utility class for representing two-dimensional positions.\\n */\\n\\n\\ngoog.provide('goog.math.Coordinate');\\n\\ngoog.require('goog.math');\\n\\n\\n\\n/**\\n * Class for representing coordinates and positions.\\n * @param {number=} opt_x Left, defaults to 0.\\n * @param {number=} opt_y Top, defaults to 0.\\n * @struct\\n * @constructor\\n */\\ngoog.math.Coordinate = function(opt_x, opt_y) {\\n  /**\\n   * X-value\\n   * @type {number}\\n   */\\n  this.x = (opt_x !== undefined) ? opt_x : 0;\\n\\n  /**\\n   * Y-value\\n   * @type {number}\\n   */\\n  this.y = (opt_y !== undefined) ? opt_y : 0;\\n};\\n\\n\\n/**\\n * Returns a new copy of the coordinate.\\n * @return {!goog.math.Coordinate} A clone of this coordinate.\\n */\\ngoog.math.Coordinate.prototype.clone = function() {\\n  return new goog.math.Coordinate(this.x, this.y);\\n};\\n\\n\\nif (goog.DEBUG) {\\n  /**\\n   * Returns a nice string representing the coordinate.\\n   * @return {string} In the form (50, 73).\\n   * @override\\n   */\\n  goog.math.Coordinate.prototype.toString = function() {\\n    return '(' + this.x + ', ' + this.y + ')';\\n  };\\n}\\n\\n\\n/**\\n * Returns whether the specified value is equal to this coordinate.\\n * @param {*} other Some other value.\\n * @return {boolean} Whether the specified value is equal to this coordinate.\\n */\\ngoog.math.Coordinate.prototype.equals = function(other) {\\n  return other instanceof goog.math.Coordinate &&\\n      goog.math.Coordinate.equals(this, other);\\n};\\n\\n\\n/**\\n * Compares coordinates for equality.\\n * @param {goog.math.Coordinate} a A Coordinate.\\n * @param {goog.math.Coordinate} b A Coordinate.\\n * @return {boolean} True iff the coordinates are equal, or if both are null.\\n */\\ngoog.math.Coordinate.equals = function(a, b) {\\n  if (a == b) {\\n    return true;\\n  }\\n  if (!a || !b) {\\n    return false;\\n  }\\n  return a.x == b.x && a.y == b.y;\\n};\\n\\n\\n/**\\n * Returns the distance between two coordinates.\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @param {!goog.math.Coordinate} b A Coordinate.\\n * @return {number} The distance between `a` and `b`.\\n */\\ngoog.math.Coordinate.distance = function(a, b) {\\n  var dx = a.x - b.x;\\n  var dy = a.y - b.y;\\n  return Math.sqrt(dx * dx + dy * dy);\\n};\\n\\n\\n/**\\n * Returns the magnitude of a coordinate.\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @return {number} The distance between the origin and `a`.\\n */\\ngoog.math.Coordinate.magnitude = function(a) {\\n  return Math.sqrt(a.x * a.x + a.y * a.y);\\n};\\n\\n\\n/**\\n * Returns the angle from the origin to a coordinate.\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @return {number} The angle, in degrees, clockwise from the positive X\\n *     axis to `a`.\\n */\\ngoog.math.Coordinate.azimuth = function(a) {\\n  return goog.math.angle(0, 0, a.x, a.y);\\n};\\n\\n\\n/**\\n * Returns the squared distance between two coordinates. Squared distances can\\n * be used for comparisons when the actual value is not required.\\n *\\n * Performance note: eliminating the square root is an optimization often used\\n * in lower-level languages, but the speed difference is not nearly as\\n * pronounced in JavaScript (only a few percent.)\\n *\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @param {!goog.math.Coordinate} b A Coordinate.\\n * @return {number} The squared distance between `a` and `b`.\\n */\\ngoog.math.Coordinate.squaredDistance = function(a, b) {\\n  var dx = a.x - b.x;\\n  var dy = a.y - b.y;\\n  return dx * dx + dy * dy;\\n};\\n\\n\\n/**\\n * Returns the difference between two coordinates as a new\\n * goog.math.Coordinate.\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @param {!goog.math.Coordinate} b A Coordinate.\\n * @return {!goog.math.Coordinate} A Coordinate representing the difference\\n *     between `a` and `b`.\\n */\\ngoog.math.Coordinate.difference = function(a, b) {\\n  return new goog.math.Coordinate(a.x - b.x, a.y - b.y);\\n};\\n\\n\\n/**\\n * Returns the sum of two coordinates as a new goog.math.Coordinate.\\n * @param {!goog.math.Coordinate} a A Coordinate.\\n * @param {!goog.math.Coordinate} b A Coordinate.\\n * @return {!goog.math.Coordinate} A Coordinate representing the sum of the two\\n *     coordinates.\\n */\\ngoog.math.Coordinate.sum = function(a, b) {\\n  return new goog.math.Coordinate(a.x + b.x, a.y + b.y);\\n};\\n\\n\\n/**\\n * Rounds the x and y fields to the next larger integer values.\\n * @return {!goog.math.Coordinate} This coordinate with ceil'd fields.\\n */\\ngoog.math.Coordinate.prototype.ceil = function() {\\n  this.x = Math.ceil(this.x);\\n  this.y = Math.ceil(this.y);\\n  return this;\\n};\\n\\n\\n/**\\n * Rounds the x and y fields to the next smaller integer values.\\n * @return {!goog.math.Coordinate} This coordinate with floored fields.\\n */\\ngoog.math.Coordinate.prototype.floor = function() {\\n  this.x = Math.floor(this.x);\\n  this.y = Math.floor(this.y);\\n  return this;\\n};\\n\\n\\n/**\\n * Rounds the x and y fields to the nearest integer values.\\n * @return {!goog.math.Coordinate} This coordinate with rounded fields.\\n */\\ngoog.math.Coordinate.prototype.round = function() {\\n  this.x = Math.round(this.x);\\n  this.y = Math.round(this.y);\\n  return this;\\n};\\n\\n\\n/**\\n * Translates this box by the given offsets. If a `goog.math.Coordinate`\\n * is given, then the x and y values are translated by the coordinate's x and y.\\n * Otherwise, x and y are translated by `tx` and `opt_ty`\\n * respectively.\\n * @param {number|goog.math.Coordinate} tx The value to translate x by or the\\n *     the coordinate to translate this coordinate by.\\n * @param {number=} opt_ty The value to translate y by.\\n * @return {!goog.math.Coordinate} This coordinate after translating.\\n */\\ngoog.math.Coordinate.prototype.translate = function(tx, opt_ty) {\\n  if (tx instanceof goog.math.Coordinate) {\\n    this.x += tx.x;\\n    this.y += tx.y;\\n  } else {\\n    this.x += Number(tx);\\n    if (typeof opt_ty === 'number') {\\n      this.y += opt_ty;\\n    }\\n  }\\n  return this;\\n};\\n\\n\\n/**\\n * Scales this coordinate by the given scale factors. The x and y values are\\n * scaled by `sx` and `opt_sy` respectively.  If `opt_sy`\\n * is not given, then `sx` is used for both x and y.\\n * @param {number} sx The scale factor to use for the x dimension.\\n * @param {number=} opt_sy The scale factor to use for the y dimension.\\n * @return {!goog.math.Coordinate} This coordinate after scaling.\\n */\\ngoog.math.Coordinate.prototype.scale = function(sx, opt_sy) {\\n  var sy = (typeof opt_sy === 'number') ? opt_sy : sx;\\n  this.x *= sx;\\n  this.y *= sy;\\n  return this;\\n};\\n\\n\\n/**\\n * Rotates this coordinate clockwise about the origin (or, optionally, the given\\n * center) by the given angle, in radians.\\n * @param {number} radians The angle by which to rotate this coordinate\\n *     clockwise about the given center, in radians.\\n * @param {!goog.math.Coordinate=} opt_center The center of rotation. Defaults\\n *     to (0, 0) if not given.\\n */\\ngoog.math.Coordinate.prototype.rotateRadians = function(radians, opt_center) {\\n  var center = opt_center || new goog.math.Coordinate(0, 0);\\n\\n  var x = this.x;\\n  var y = this.y;\\n  var cos = Math.cos(radians);\\n  var sin = Math.sin(radians);\\n\\n  this.x = (x - center.x) * cos - (y - center.y) * sin + center.x;\\n  this.y = (x - center.x) * sin + (y - center.y) * cos + center.y;\\n};\\n\\n\\n/**\\n * Rotates this coordinate clockwise about the origin (or, optionally, the given\\n * center) by the given angle, in degrees.\\n * @param {number} degrees The angle by which to rotate this coordinate\\n *     clockwise about the given center, in degrees.\\n * @param {!goog.math.Coordinate=} opt_center The center of rotation. Defaults\\n *     to (0, 0) if not given.\\n */\\ngoog.math.Coordinate.prototype.rotateDegrees = function(degrees, opt_center) {\\n  this.rotateRadians(goog.math.toRadians(degrees), opt_center);\\n};\\n\"],\n\"names\":[\"goog\",\"provide\",\"require\",\"math\",\"Coordinate\",\"goog.math.Coordinate\",\"opt_x\",\"opt_y\",\"x\",\"undefined\",\"y\",\"prototype\",\"clone\",\"goog.math.Coordinate.prototype.clone\",\"DEBUG\",\"toString\",\"goog.math.Coordinate.prototype.toString\",\"equals\",\"goog.math.Coordinate.prototype.equals\",\"other\",\"goog.math.Coordinate.equals\",\"a\",\"b\",\"distance\",\"goog.math.Coordinate.distance\",\"dx\",\"dy\",\"Math\",\"sqrt\",\"magnitude\",\"goog.math.Coordinate.magnitude\",\"azimuth\",\"goog.math.Coordinate.azimuth\",\"angle\",\"squaredDistance\",\"goog.math.Coordinate.squaredDistance\",\"difference\",\"goog.math.Coordinate.difference\",\"sum\",\"goog.math.Coordinate.sum\",\"ceil\",\"goog.math.Coordinate.prototype.ceil\",\"floor\",\"goog.math.Coordinate.prototype.floor\",\"round\",\"goog.math.Coordinate.prototype.round\",\"translate\",\"goog.math.Coordinate.prototype.translate\",\"tx\",\"opt_ty\",\"Number\",\"scale\",\"goog.math.Coordinate.prototype.scale\",\"sx\",\"opt_sy\",\"sy\",\"rotateRadians\",\"goog.math.Coordinate.prototype.rotateRadians\",\"radians\",\"opt_center\",\"center\",\"cos\",\"sin\",\"rotateDegrees\",\"goog.math.Coordinate.prototype.rotateDegrees\",\"degrees\",\"toRadians\"]\n}\n"]