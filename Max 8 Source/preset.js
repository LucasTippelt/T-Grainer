autowatch = 1;
inlets = 1;
outlets=0;
var starts1g1;
var ends1g1;
var starts1g2;
var ends1g2;
var starts1g3;
var ends1g3;
var starts1g4;
var ends1g4;
var starts2g1;
var ends2g1;
var starts2g2;
var ends2g2;
var starts2g3;
var ends2g3;
var starts2g4;
var ends2g4;
var starts3g1;
var ends3g1;
var starts3g2;
var ends3g2;
var starts3g3;
var ends3g3;
var starts3g4;
var ends3g4;
var pianoroll;
var bpm;

 
var memstr;
var mem = new Object();
var UI = new Object();

function clear(){
	UI = new Object();
    
}

function write(p){
	var jase = JSON.stringify(UI,null,'\t');
	var path = p;
	var fout = new File(path,"write","TEXT");
	if (fout.isopen) {
			fout.writeline(jase);
			fout.close();
		
		} else {
			
		}
}


//for testing - shows all the current objects in Max window
function dump(){
	for(i in UI){
		for(j in UI[i]){
			post("\n",i,j,UI[i][j]);
		}
	}
}

//fill UI with some stuff:
function anything(){
	var a = arrayfromargs(messagename,arguments);
	var id = a[0];
	var property = a[1];
	var data = a.slice(2);
	//post("\n",id,",",property,",",data);
	if(UI[id]==null){
		UI[id]=new Object();
		//post("\nnew UI");
	}
	UI[id][property] = data;	
}

autowatch = 1;







// Create a JSON object only if one does not already exist. We create the
// methods in a closure to avoid creating global variables.

var JSON;
if (!JSON) {
    JSON = {};
}

(function () {
    "use strict";

    function f(n) {
        // Format integers to have at least two digits.
        return n < 10 ? '0' + n : n;
    }

    if (typeof Date.prototype.toJSON !== 'function') {

        Date.prototype.toJSON = function (key) {

            return isFinite(this.valueOf()) ?
                this.getUTCFullYear()     + '-' +
                f(this.getUTCMonth() + 1) + '-' +
                f(this.getUTCDate())      + 'T' +
                f(this.getUTCHours())     + ':' +
                f(this.getUTCMinutes())   + ':' +
                f(this.getUTCSeconds())   + 'Z' : null;
        };

        String.prototype.toJSON      =
            Number.prototype.toJSON  =
            Boolean.prototype.toJSON = function (key) {
                return this.valueOf();
            };
    }

    var cx = /[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
        escapable = /[\\\"\x00-\x1f\x7f-\x9f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g,
        gap,
        indent,
        meta = {    // table of character substitutions
            '\b': '\\b',
            '\t': '\\t',
            '\n': '\\n',
            '\f': '\\f',
            '\r': '\\r',
            '"' : '\\"',
            '\\': '\\\\'
        },
        rep;


    function quote(string) {

// If the string contains no control characters, no quote characters, and no
// backslash characters, then we can safely slap some quotes around it.
// Otherwise we must also replace the offending characters with safe escape
// sequences.

        escapable.lastIndex = 0;
        return escapable.test(string) ? '"' + string.replace(escapable, function (a) {
            var c = meta[a];
            return typeof c === 'string' ? c :
                '\\u' + ('0000' + a.charCodeAt(0).toString(16)).slice(-4);
        }) + '"' : '"' + string + '"';
    }


    function str(key, holder) {

// Produce a string from holder[key].

        var i,          // The loop counter.
            k,          // The member key.
            v,          // The member value.
            length,
            mind = gap,
            partial,
            value = holder[key];

// If the value has a toJSON method, call it to obtain a replacement value.

        if (value && typeof value === 'object' &&
                typeof value.toJSON === 'function') {
            value = value.toJSON(key);
        }

// If we were called with a replacer function, then call the replacer to
// obtain a replacement value.

        if (typeof rep === 'function') {
            value = rep.call(holder, key, value);
        }

// What happens next depends on the value's type.

        switch (typeof value) {
        case 'string':
            return quote(value);

        case 'number':

// JSON numbers must be finite. Encode non-finite numbers as null.

            return isFinite(value) ? String(value) : 'null';

        case 'boolean':
        case 'null':

// If the value is a boolean or null, convert it to a string. Note:
// typeof null does not produce 'null'. The case is included here in
// the remote chance that this gets fixed someday.

            return String(value);

// If the type is 'object', we might be dealing with an object or an array or
// null.

        case 'object':

// Due to a specification blunder in ECMAScript, typeof null is 'object',
// so watch out for that case.

            if (!value) {
                return 'null';
            }

// Make an array to hold the partial results of stringifying this object value.

            gap += indent;
            partial = [];

// Is the value an array?

            if (Object.prototype.toString.apply(value) === '[object Array]') {

// The value is an array. Stringify every element. Use null as a placeholder
// for non-JSON values.

                length = value.length;
                for (i = 0; i < length; i += 1) {
                    partial[i] = str(i, value) || 'null';
                }

// Join all of the elements together, separated with commas, and wrap them in
// brackets.

                v = partial.length === 0 ? '[]' : gap ?
                    '[\n' + gap + partial.join(',\n' + gap) + '\n' + mind + ']' :
                    '[' + partial.join(',') + ']';
                gap = mind;
                return v;
            }

// If the replacer is an array, use it to select the members to be stringified.

            if (rep && typeof rep === 'object') {
                length = rep.length;
                for (i = 0; i < length; i += 1) {
                    if (typeof rep[i] === 'string') {
                        k = rep[i];
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (gap ? ': ' : ':') + v);
                        }
                    }
                }
            } else {

// Otherwise, iterate through all of the keys in the object.

                for (k in value) {
                    if (Object.prototype.hasOwnProperty.call(value, k)) {
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (gap ? ': ' : ':') + v);
                        }
                    }
                }
            }

// Join all of the member texts together, separated with commas,
// and wrap them in braces.

            v = partial.length === 0 ? '{}' : gap ?
                '{\n' + gap + partial.join(',\n' + gap) + '\n' + mind + '}' :
                '{' + partial.join(',') + '}';
            gap = mind;
            return v;
        }
    }

// If the JSON object does not yet have a stringify method, give it one.

    if (typeof JSON.stringify !== 'function') {
        JSON.stringify = function (value, replacer, space) {

// The stringify method takes a value and an optional replacer, and an optional
// space parameter, and returns a JSON text. The replacer can be a function
// that can replace values, or an array of strings that will select the keys.
// A default replacer method can be provided. Use of the space parameter can
// produce text that is more easily readable.

            var i;
            gap = '';
            indent = '';

// If the space parameter is a number, make an indent string containing that
// many spaces.

            if (typeof space === 'number') {
                for (i = 0; i < space; i += 1) {
                    indent += ' ';
                }

// If the space parameter is a string, it will be used as the indent string.

            } else if (typeof space === 'string') {
                indent = space;
            }

// If there is a replacer, it must be a function or an array.
// Otherwise, throw an error.

            rep = replacer;
            if (replacer && typeof replacer !== 'function' &&
                    (typeof replacer !== 'object' ||
                    typeof replacer.length !== 'number')) {
                throw new Error('JSON.stringify');
            }

// Make a fake root object containing our value under the key of ''.
// Return the result of stringifying the value.

            return str('', {'': value});
        };
    }


// If the JSON object does not yet have a parse method, give it one.

    if (typeof JSON.parse !== 'function') {
        JSON.parse = function (text, reviver) {

// The parse method takes a text and an optional reviver function, and returns
// a JavaScript value if the text is a valid JSON text.

            var j;

            function walk(holder, key) {

// The walk method is used to recursively walk the resulting structure so
// that modifications can be made.

                var k, v, value = holder[key];
                if (value && typeof value === 'object') {
                    for (k in value) {
                        if (Object.prototype.hasOwnProperty.call(value, k)) {
                            v = walk(value, k);
                            if (v !== undefined) {
                                value[k] = v;
                            } else {
                                delete value[k];
                            }
                        }
                    }
                }
                return reviver.call(holder, key, value);
            }


// Parsing happens in four stages. In the first stage, we replace certain
// Unicode characters with escape sequences. JavaScript handles many characters
// incorrectly, either silently deleting them, or treating them as line endings.

            text = String(text);
            cx.lastIndex = 0;
            if (cx.test(text)) {
                text = text.replace(cx, function (a) {
                    return '\\u' +
                        ('0000' + a.charCodeAt(0).toString(16)).slice(-4);
                });
            }

// In the second stage, we run the text against regular expressions that look
// for non-JSON patterns. We are especially concerned with '()' and 'new'
// because they can cause invocation, and '=' because it can cause mutation.
// But just to be safe, we want to reject all unexpected forms.

// We split the second stage into 4 regexp operations in order to work around
// crippling inefficiencies in IE's and Safari's regexp engines. First we
// replace the JSON backslash pairs with '@' (a non-JSON character). Second, we
// replace all simple value tokens with ']' characters. Third, we delete all
// open brackets that follow a colon or comma or that begin the text. Finally,
// we look to see that the remaining characters are only whitespace or ']' or
// ',' or ':' or '{' or '}'. If that is so, then the text is safe for eval.

            if (/^[\],:{}\s]*$/
                    .test(text.replace(/\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g, '@')
                        .replace(/"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g, ']')
                        .replace(/(?:^|:|,)(?:\s*\[)+/g, ''))) {

// In the third stage we use the eval function to compile the text into a
// JavaScript structure. The '{' operator is subject to a syntactic ambiguity
// in JavaScript: it can begin a block or an object literal. We wrap the text
// in parens to eliminate the ambiguity.

                j = eval('(' + text + ')');

// In the optional fourth stage, we recursively walk the new structure, passing
// each name/value pair to a reviver function for possible transformation.

                return typeof reviver === 'function' ?
                    walk({'': j}, '') : j;
            }

// If the text is not JSON parseable, then a SyntaxError is thrown.

            throw new SyntaxError('JSON.parse');
        };
    }
}());
/*=======================================================================*/
// EOF



var s1g1b1;
var s1g1b2;
var s1g1b3;
var s1g1b4;
var s1g1b5;
var s1g1b6;
var s1g1b7;
var s1g1b8;
var s1g1b9;
var s1g1b10;
var s1g1b11;
var s1g1b12;
var s1g1b13;
var s1g1b14;
var s1g1b15;
var s1g1b16;
function beat1s1g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g1b1=v1;    
s1g1b2=v2;
s1g1b3=v3;
s1g1b4=v4;
s1g1b5=v5;
s1g1b6=v6;
s1g1b7=v7;
s1g1b8=v8;
}
function beat2s1g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g1b9=v1;    
s1g1b10=v2;
s1g1b11=v3;
s1g1b12=v4;
s1g1b13=v5;
s1g1b14=v6;
s1g1b15=v7;
s1g1b16=v8;
}
var s1g2b1;
var s1g2b2;
var s1g2b3;
var s1g2b4;
var s1g2b5;
var s1g2b6;
var s1g2b7;
var s1g2b8;
var s1g2b9;
var s1g2b10;
var s1g2b11;
var s1g2b12;
var s1g2b13;
var s1g2b14;
var s1g2b15;
var s1g2b16;
function beat1s1g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g2b1=v1;    
s1g2b2=v2;
s1g2b3=v3;
s1g2b4=v4;
s1g2b5=v5;
s1g2b6=v6;
s1g2b7=v7;
s1g2b8=v8;
}
function beat2s1g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g2b9=v1;    
s1g2b10=v2;
s1g2b11=v3;
s1g2b12=v4;
s1g2b13=v5;
s1g2b14=v6;
s1g2b15=v7;
s1g2b16=v8;
}
var s1g3b1;
var s1g3b2;
var s1g3b3;
var s1g3b4;
var s1g3b5;
var s1g3b6;
var s1g3b7;
var s1g3b8;
var s1g3b9;
var s1g3b10;
var s1g3b11;
var s1g3b12;
var s1g3b13;
var s1g3b14;
var s1g3b15;
var s1g3b16;
function beat1s1g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g3b1=v1;    
s1g3b2=v2;
s1g3b3=v3;
s1g3b4=v4;
s1g3b5=v5;
s1g3b6=v6;
s1g3b7=v7;
s1g3b8=v8;
}
function beat2s1g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g3b9=v1;    
s1g3b10=v2;
s1g3b11=v3;
s1g3b12=v4;
s1g3b13=v5;
s1g3b14=v6;
s1g3b15=v7;
s1g3b16=v8;
}
var s1g4b1;
var s1g4b2;
var s1g4b3;
var s1g4b4;
var s1g4b5;
var s1g4b6;
var s1g4b7;
var s1g4b8;
var s1g4b9;
var s1g4b10;
var s1g4b11;
var s1g4b12;
var s1g4b13;
var s1g4b14;
var s1g4b15;
var s1g4b16;
function beat1s1g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g4b1=v1;    
s1g4b2=v2;
s1g4b3=v3;
s1g4b4=v4;
s1g4b5=v5;
s1g4b6=v6;
s1g4b7=v7;
s1g4b8=v8;
}
function beat2s1g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s1g4b9=v1;    
s1g4b10=v2;
s1g4b11=v3;
s1g4b12=v4;
s1g4b13=v5;
s1g4b14=v6;
s1g4b15=v7;
s1g4b16=v8;
}
var s2g1b1;
var s2g1b2;
var s2g1b3;
var s2g1b4;
var s2g1b5;
var s2g1b6;
var s2g1b7;
var s2g1b8;
var s2g1b9;
var s2g1b10;
var s2g1b11;
var s2g1b12;
var s2g1b13;
var s2g1b14;
var s2g1b15;
var s2g1b16;
function beat1s2g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g1b1=v1;    
s2g1b2=v2;
s2g1b3=v3;
s2g1b4=v4;
s2g1b5=v5;
s2g1b6=v6;
s2g1b7=v7;
s2g1b8=v8;
}
function beat2s2g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g1b9=v1;    
s2g1b10=v2;
s2g1b11=v3;
s2g1b12=v4;
s2g1b13=v5;
s2g1b14=v6;
s2g1b15=v7;
s2g1b16=v8;
}
var s2g2b1;
var s2g2b2;
var s2g2b3;
var s2g2b4;
var s2g2b5;
var s2g2b6;
var s2g2b7;
var s2g2b8;
var s2g2b9;
var s2g2b10;
var s2g2b11;
var s2g2b12;
var s2g2b13;
var s2g2b14;
var s2g2b15;
var s2g2b16;
function beat1s2g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g2b1=v1;    
s2g2b2=v2;
s2g2b3=v3;
s2g2b4=v4;
s2g2b5=v5;
s2g2b6=v6;
s2g2b7=v7;
s2g2b8=v8;
}
function beat2s2g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g2b9=v1;    
s2g2b10=v2;
s2g2b11=v3;
s2g2b12=v4;
s2g2b13=v5;
s2g2b14=v6;
s2g2b15=v7;
s2g2b16=v8;
}
var s2g3b1;
var s2g3b2;
var s2g3b3;
var s2g3b4;
var s2g3b5;
var s2g3b6;
var s2g3b7;
var s2g3b8;
var s2g3b9;
var s2g3b10;
var s2g3b11;
var s2g3b12;
var s2g3b13;
var s2g3b14;
var s2g3b15;
var s2g3b16;
function beat1s2g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g3b1=v1;    
s2g3b2=v2;
s2g3b3=v3;
s2g3b4=v4;
s2g3b5=v5;
s2g3b6=v6;
s2g3b7=v7;
s2g3b8=v8;
}
function beat2s2g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g3b9=v1;    
s2g3b10=v2;
s2g3b11=v3;
s2g3b12=v4;
s2g3b13=v5;
s2g3b14=v6;
s2g3b15=v7;
s2g3b16=v8;
}
var s2g4b1;
var s2g4b2;
var s2g4b3;
var s2g4b4;
var s2g4b5;
var s2g4b6;
var s2g4b7;
var s2g4b8;
var s2g4b9;
var s2g4b10;
var s2g4b11;
var s2g4b12;
var s2g4b13;
var s2g4b14;
var s2g4b15;
var s2g4b16;
function beat1s2g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g4b1=v1;    
s2g4b2=v2;
s2g4b3=v3;
s2g4b4=v4;
s2g4b5=v5;
s2g4b6=v6;
s2g4b7=v7;
s2g4b8=v8;
}
function beat2s2g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s2g4b9=v1;    
s2g4b10=v2;
s2g4b11=v3;
s2g4b12=v4;
s2g4b13=v5;
s2g4b14=v6;
s2g4b15=v7;
s2g4b16=v8;
}
var s3g1b1;
var s3g1b2;
var s3g1b3;
var s3g1b4;
var s3g1b5;
var s3g1b6;
var s3g1b7;
var s3g1b8;
var s3g1b9;
var s3g1b10;
var s3g1b11;
var s3g1b12;
var s3g1b13;
var s3g1b14;
var s3g1b15;
var s3g1b16;
function beat1s3g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g1b1=v1;    
s3g1b2=v2;
s3g1b3=v3;
s3g1b4=v4;
s3g1b5=v5;
s3g1b6=v6;
s3g1b7=v7;
s3g1b8=v8;
}
function beat2s3g1(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g1b9=v1;    
s3g1b10=v2;
s3g1b11=v3;
s3g1b12=v4;
s3g1b13=v5;
s3g1b14=v6;
s3g1b15=v7;
s3g1b16=v8;
}
var s3g2b1;
var s3g2b2;
var s3g2b3;
var s3g2b4;
var s3g2b5;
var s3g2b6;
var s3g2b7;
var s3g2b8;
var s3g2b9;
var s3g2b10;
var s3g2b11;
var s3g2b12;
var s3g2b13;
var s3g2b14;
var s3g2b15;
var s3g2b16;
function beat1s3g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g2b1=v1;    
s3g2b2=v2;
s3g2b3=v3;
s3g2b4=v4;
s3g2b5=v5;
s3g2b6=v6;
s3g2b7=v7;
s3g2b8=v8;
}
function beat2s3g2(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g2b9=v1;    
s3g2b10=v2;
s3g2b11=v3;
s3g2b12=v4;
s3g2b13=v5;
s3g2b14=v6;
s3g2b15=v7;
s3g2b16=v8;
}
var s3g3b1;
var s3g3b2;
var s3g3b3;
var s3g3b4;
var s3g3b5;
var s3g3b6;
var s3g3b7;
var s3g3b8;
var s3g3b9;
var s3g3b10;
var s3g3b11;
var s3g3b12;
var s3g3b13;
var s3g3b14;
var s3g3b15;
var s3g3b16;
function beat1s3g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g3b1=v1;    
s3g3b2=v2;
s3g3b3=v3;
s3g3b4=v4;
s3g3b5=v5;
s3g3b6=v6;
s3g3b7=v7;
s3g3b8=v8;
}
function beat2s3g3(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g3b9=v1;    
s3g3b10=v2;
s3g3b11=v3;
s3g3b12=v4;
s3g3b13=v5;
s3g3b14=v6;
s3g3b15=v7;
s3g3b16=v8;
}
var s3g4b1;
var s3g4b2;
var s3g4b3;
var s3g4b4;
var s3g4b5;
var s3g4b6;
var s3g4b7;
var s3g4b8;
var s3g4b9;
var s3g4b10;
var s3g4b11;
var s3g4b12;
var s3g4b13;
var s3g4b14;
var s3g4b15;
var s3g4b16;
function beat1s3g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g4b1=v1;    
s3g4b2=v2;
s3g4b3=v3;
s3g4b4=v4;
s3g4b5=v5;
s3g4b6=v6;
s3g4b7=v7;
s3g4b8=v8;
}
function beat2s3g4(v1, v2, v3, v4, v5, v6, v7, v8)
{
s3g4b9=v1;    
s3g4b10=v2;
s3g4b11=v3;
s3g4b12=v4;
s3g4b13=v5;
s3g4b14=v6;
s3g4b15=v7;
s3g4b16=v8;
}
function saves1g1(v1, v2) 
{   starts1g1 = v1;
    ends1g1 = v2;
    
}
function saves1g2(v1, v2) 
{   starts1g2 = v1;
    ends1g2 = v2;
    
}
function saves1g3(v1, v2) 
{   starts1g3 = v1;
    ends1g3 = v2;
    
}
function saves1g4(v1, v2) 
{   starts1g4 = v1;
    ends1g4 = v2;
    
}
function saves2g1(v1, v2) 
{   starts2g1 = v1;
    ends2g1 = v2;
    
}
function saves2g2(v1, v2) 
{   starts2g2 = v1;
    ends2g2 = v2;
    
}
function saves2g3(v1, v2) 
{   starts2g3 = v1;
    ends2g3 = v2;
    
}
function saves2g4(v1, v2) 
{   starts2g4 = v1;
    ends2g4 = v2;
    
}
function saves3g1(v1, v2) 
{   starts3g1 = v1;
    ends3g1 = v2;
}
function saves3g2(v1, v2) 
{   starts3g2 = v1;
    ends3g2 = v2;
}
function saves3g3(v1, v2) 
{   starts3g3 = v1;
    ends3g3 = v2;
}
function saves3g4(v1, v2) 
{   starts3g4 = v1;
    ends3g4 = v2;
}
function savebpm(v1, v2) 
{
    pianoroll = v1;
    bpm = v2;
}


var mutes1g1;
var envelopes1g1;
var reverses1g1;
var togglelps1g1;
var togglehps1g1;
var gains1g1;
var lfogains1g1;
var pitchs1g1;
var lfopitchs1g1;
var pans1g1;
var lfopans1g1;
var lps1g1;
var lfolps1g1;
var hps1g1;
var lfohps1g1;
var del_time_s1g1
var del_toggle_s1g1
var lfodel_time_s1g1;
var del_feedback_s1g1;
var lfodel_feedback_s1g1;
var del_drywet_s1g1;
var lfodel_drywet_s1g1;
var flang_toggle_s1g1;
var flang_feedback_s1g1;
var lfoflang_feedback_s1g1;
var flang_rate_s1g1;
var lfoflang_rate_s1g1;
var flang_drywet_s1g1;
var lfoflang_drywet_s1g1;
var rev_toggle_s1g1;
var rev_decay_s1g1;
var lforev_decay_s1g1;
var rev_drywet_s1g1;
var lforev_drywet_s1g1;
function s1g1par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes1g1=v1;
    envelopes1g1=v2;
    reverses1g1=v3;
    togglelps1g1=v4;
    togglehps1g1=v5;
    gains1g1=v6;
    lfogains1g1=v7;
}
function s1g1par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs1g1=v1;
    lfopitchs1g1=v2;
    pans1g1=v3;
    lfopans1g1=v4;
    lps1g1=v5;
    lfolps1g1=v6;
    hps1g1=v7;
    lfohps1g1=v8;
}
function s1g1del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s1g1=v1;
    del_time_s1g1=v2;
    lfodel_time_s1g1=v3;
    del_feedback_s1g1=v4;
    lfodel_feedback_s1g1=v5;
    del_drywet_s1g1=v6;
    lfodel_drywet_s1g1=v7;
}
function s1g1flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s1g1=v1;
    flang_feedback_s1g1=v2;
    lfoflang_feedback_s1g1=v3;
    flang_rate_s1g1=v4;
    lfoflang_rate_s1g1=v5;
    flang_drywet_s1g1=v6;
    lfoflang_drywet_s1g1=v7;
}

var sample1name;
var sample2name;
var sample3name;
function samplenames(v1,v2,v3)
{
sample1name=v1;
sample2name=v2;
sample3name=v3;
}

function s1g1rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s1g1=v1;
    rev_decay_s1g1=v2;
    lforev_decay_s1g1=v3;
    rev_drywet_s1g1=v4;
    lforev_drywet_s1g1=v5;
}
var mutes1g2;
var envelopes1g2;
var reverses1g2;
var togglelps1g2;
var togglehps1g2;
var gains1g2;
var lfogains1g2;
var pitchs1g2;
var lfopitchs1g2;
var pans1g2;
var lfopans1g2;
var lps1g2;
var lfolps1g2;
var hps1g2;
var lfohps1g2;
var del_time_s1g2
var del_toggle_s1g2
var lfodel_time_s1g2;
var del_feedback_s1g2;
var lfodel_feedback_s1g2;
var del_drywet_s1g2;
var lfodel_drywet_s1g2;
var flang_toggle_s1g2;
var flang_feedback_s1g2;
var lfoflang_feedback_s1g2;
var flang_rate_s1g2;
var lfoflang_rate_s1g2;
var flang_drywet_s1g2;
var lfoflang_drywet_s1g2;
var rev_toggle_s1g2;
var rev_decay_s1g2;
var lforev_decay_s1g2;
var rev_drywet_s1g2;
var lforev_drywet_s1g2;
function s1g2par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes1g2=v1;
    envelopes1g2=v2;
    reverses1g2=v3;
    togglelps1g2=v4;
    togglehps1g2=v5;
    gains1g2=v6;
    lfogains1g2=v7;
}
function s1g2par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs1g2=v1;
    lfopitchs1g2=v2;
    pans1g2=v3;
    lfopans1g2=v4;
    lps1g2=v5;
    lfolps1g2=v6;
    hps1g2=v7;
    lfohps1g2=v8;
}
function s1g2del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s1g2=v1;
    del_time_s1g2=v2;
    lfodel_time_s1g2=v3;
    del_feedback_s1g2=v4;
    lfodel_feedback_s1g2=v5;
    del_drywet_s1g2=v6;
    lfodel_drywet_s1g2=v7;
}
function s1g2flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s1g2=v1;
    flang_feedback_s1g2=v2;
    lfoflang_feedback_s1g2=v3;
    flang_rate_s1g2=v4;
    lfoflang_rate_s1g2=v5;
    flang_drywet_s1g2=v6;
    lfoflang_drywet_s1g2=v7;
}
function s1g2rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s1g2=v1;
    rev_decay_s1g2=v2;
    lforev_decay_s1g2=v3;
    rev_drywet_s1g2=v4;
    lforev_drywet_s1g2=v5;
}
var mutes1g3;
var envelopes1g3;
var reverses1g3;
var togglelps1g3;
var togglehps1g3;
var gains1g3;
var lfogains1g3;
var pitchs1g3;
var lfopitchs1g3;
var pans1g3;
var lfopans1g3;
var lps1g3;
var lfolps1g3;
var hps1g3;
var lfohps1g3;
var del_time_s1g3
var del_toggle_s1g3
var lfodel_time_s1g3;
var del_feedback_s1g3;
var lfodel_feedback_s1g3;
var del_drywet_s1g3;
var lfodel_drywet_s1g3;
var flang_toggle_s1g3;
var flang_feedback_s1g3;
var lfoflang_feedback_s1g3;
var flang_rate_s1g3;
var lfoflang_rate_s1g3;
var flang_drywet_s1g3;
var lfoflang_drywet_s1g3;
var rev_toggle_s1g3;
var rev_decay_s1g3;
var lforev_decay_s1g3;
var rev_drywet_s1g3;
var lforev_drywet_s1g3;
function s1g3par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes1g3=v1;
    envelopes1g3=v2;
    reverses1g3=v3;
    togglelps1g3=v4;
    togglehps1g3=v5;
    gains1g3=v6;
    lfogains1g3=v7;
}
function s1g3par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs1g3=v1;
    lfopitchs1g3=v2;
    pans1g3=v3;
    lfopans1g3=v4;
    lps1g3=v5;
    lfolps1g3=v6;
    hps1g3=v7;
    lfohps1g3=v8;
}
function s1g3del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s1g3=v1;
    del_time_s1g3=v2;
    lfodel_time_s1g3=v3;
    del_feedback_s1g3=v4;
    lfodel_feedback_s1g3=v5;
    del_drywet_s1g3=v6;
    lfodel_drywet_s1g3=v7;
}
function s1g3flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s1g3=v1;
    flang_feedback_s1g3=v2;
    lfoflang_feedback_s1g3=v3;
    flang_rate_s1g3=v4;
    lfoflang_rate_s1g3=v5;
    flang_drywet_s1g3=v6;
    lfoflang_drywet_s1g3=v7;
}
function s1g3rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s1g3=v1;
    rev_decay_s1g3=v2;
    lforev_decay_s1g3=v3;
    rev_drywet_s1g3=v4;
    lforev_drywet_s1g3=v5;
}
var mutes1g4;
var envelopes1g4;
var reverses1g4;
var togglelps1g4;
var togglehps1g4;
var gains1g4;
var lfogains1g4;
var pitchs1g4;
var lfopitchs1g4;
var pans1g4;
var lfopans1g4;
var lps1g4;
var lfolps1g4;
var hps1g4;
var lfohps1g4;
var del_time_s1g4
var del_toggle_s1g4
var lfodel_time_s1g4;
var del_feedback_s1g4;
var lfodel_feedback_s1g4;
var del_drywet_s1g4;
var lfodel_drywet_s1g4;
var flang_toggle_s1g4;
var flang_feedback_s1g4;
var lfoflang_feedback_s1g4;
var flang_rate_s1g4;
var lfoflang_rate_s1g4;
var flang_drywet_s1g4;
var lfoflang_drywet_s1g4;
var rev_toggle_s1g4;
var rev_decay_s1g4;
var lforev_decay_s1g4;
var rev_drywet_s1g4;
var lforev_drywet_s1g4;
function s1g4par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes1g4=v1;
    envelopes1g4=v2;
    reverses1g4=v3;
    togglelps1g4=v4;
    togglehps1g4=v5;
    gains1g4=v6;
    lfogains1g4=v7;
}
function s1g4par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs1g4=v1;
    lfopitchs1g4=v2;
    pans1g4=v3;
    lfopans1g4=v4;
    lps1g4=v5;
    lfolps1g4=v6;
    hps1g4=v7;
    lfohps1g4=v8;
}
function s1g4del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s1g4=v1;
    del_time_s1g4=v2;
    lfodel_time_s1g4=v3;
    del_feedback_s1g4=v4;
    lfodel_feedback_s1g4=v5;
    del_drywet_s1g4=v6;
    lfodel_drywet_s1g4=v7;
}
function s1g4flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s1g4=v1;
    flang_feedback_s1g4=v2;
    lfoflang_feedback_s1g4=v3;
    flang_rate_s1g4=v4;
    lfoflang_rate_s1g4=v5;
    flang_drywet_s1g4=v6;
    lfoflang_drywet_s1g4=v7;
}
function s1g4rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s1g4=v1;
    rev_decay_s1g4=v2;
    lforev_decay_s1g4=v3;
    rev_drywet_s1g4=v4;
    lforev_drywet_s1g4=v5;
}
var mutes2g1;
var envelopes2g1;
var reverses2g1;
var togglelps2g1;
var togglehps2g1;
var gains2g1;
var lfogains2g1;
var pitchs2g1;
var lfopitchs2g1;
var pans2g1;
var lfopans2g1;
var lps2g1;
var lfolps2g1;
var hps2g1;
var lfohps2g1;
var del_time_s2g1
var del_toggle_s2g1
var lfodel_time_s2g1;
var del_feedback_s2g1;
var lfodel_feedback_s2g1;
var del_drywet_s2g1;
var lfodel_drywet_s2g1;
var flang_toggle_s2g1;
var flang_feedback_s2g1;
var lfoflang_feedback_s2g1;
var flang_rate_s2g1;
var lfoflang_rate_s2g1;
var flang_drywet_s2g1;
var lfoflang_drywet_s2g1;
var rev_toggle_s2g1;
var rev_decay_s2g1;
var lforev_decay_s2g1;
var rev_drywet_s2g1;
var lforev_drywet_s2g1;
function s2g1par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes2g1=v1;
    envelopes2g1=v2;
    reverses2g1=v3;
    togglelps2g1=v4;
    togglehps2g1=v5;
    gains2g1=v6;
    lfogains2g1=v7;
}
function s2g1par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs2g1=v1;
    lfopitchs2g1=v2;
    pans2g1=v3;
    lfopans2g1=v4;
    lps2g1=v5;
    lfolps2g1=v6;
    hps2g1=v7;
    lfohps2g1=v8;
}
function s2g1del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s2g1=v1;
    del_time_s2g1=v2;
    lfodel_time_s2g1=v3;
    del_feedback_s2g1=v4;
    lfodel_feedback_s2g1=v5;
    del_drywet_s2g1=v6;
    lfodel_drywet_s2g1=v7;
}
function s2g1flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s2g1=v1;
    flang_feedback_s2g1=v2;
    lfoflang_feedback_s2g1=v3;
    flang_rate_s2g1=v4;
    lfoflang_rate_s2g1=v5;
    flang_drywet_s2g1=v6;
    lfoflang_drywet_s2g1=v7;
}
function s2g1rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s2g1=v1;
    rev_decay_s2g1=v2;
    lforev_decay_s2g1=v3;
    rev_drywet_s2g1=v4;
    lforev_drywet_s2g1=v5;
}
var mutes2g2;
var envelopes2g2;
var reverses2g2;
var togglelps2g2;
var togglehps2g2;
var gains2g2;
var lfogains2g2;
var pitchs2g2;
var lfopitchs2g2;
var pans2g2;
var lfopans2g2;
var lps2g2;
var lfolps2g2;
var hps2g2;
var lfohps2g2;
var del_time_s2g2
var del_toggle_s2g2
var lfodel_time_s2g2;
var del_feedback_s2g2;
var lfodel_feedback_s2g2;
var del_drywet_s2g2;
var lfodel_drywet_s2g2;
var flang_toggle_s2g2;
var flang_feedback_s2g2;
var lfoflang_feedback_s2g2;
var flang_rate_s2g2;
var lfoflang_rate_s2g2;
var flang_drywet_s2g2;
var lfoflang_drywet_s2g2;
var rev_toggle_s2g2;
var rev_decay_s2g2;
var lforev_decay_s2g2;
var rev_drywet_s2g2;
var lforev_drywet_s2g2;
function s2g2par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes2g2=v1;
    envelopes2g2=v2;
    reverses2g2=v3;
    togglelps2g2=v4;
    togglehps2g2=v5;
    gains2g2=v6;
    lfogains2g2=v7;
}
function s2g2par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs2g2=v1;
    lfopitchs2g2=v2;
    pans2g2=v3;
    lfopans2g2=v4;
    lps2g2=v5;
    lfolps2g2=v6;
    hps2g2=v7;
    lfohps2g2=v8;
}
function s2g2del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s2g2=v1;
    del_time_s2g2=v2;
    lfodel_time_s2g2=v3;
    del_feedback_s2g2=v4;
    lfodel_feedback_s2g2=v5;
    del_drywet_s2g2=v6;
    lfodel_drywet_s2g2=v7;
}
function s2g2flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s2g2=v1;
    flang_feedback_s2g2=v2;
    lfoflang_feedback_s2g2=v3;
    flang_rate_s2g2=v4;
    lfoflang_rate_s2g2=v5;
    flang_drywet_s2g2=v6;
    lfoflang_drywet_s2g2=v7;
}
function s2g2rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s2g2=v1;
    rev_decay_s2g2=v2;
    lforev_decay_s2g2=v3;
    rev_drywet_s2g2=v4;
    lforev_drywet_s2g2=v5;
}
var mutes2g3;
var envelopes2g3;
var reverses2g3;
var togglelps2g3;
var togglehps2g3;
var gains2g3;
var lfogains2g3;
var pitchs2g3;
var lfopitchs2g3;
var pans2g3;
var lfopans2g3;
var lps2g3;
var lfolps2g3;
var hps2g3;
var lfohps2g3;
var del_time_s2g3
var del_toggle_s2g3
var lfodel_time_s2g3;
var del_feedback_s2g3;
var lfodel_feedback_s2g3;
var del_drywet_s2g3;
var lfodel_drywet_s2g3;
var flang_toggle_s2g3;
var flang_feedback_s2g3;
var lfoflang_feedback_s2g3;
var flang_rate_s2g3;
var lfoflang_rate_s2g3;
var flang_drywet_s2g3;
var lfoflang_drywet_s2g3;
var rev_toggle_s2g3;
var rev_decay_s2g3;
var lforev_decay_s2g3;
var rev_drywet_s2g3;
var lforev_drywet_s2g3;
function s2g3par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes2g3=v1;
    envelopes2g3=v2;
    reverses2g3=v3;
    togglelps2g3=v4;
    togglehps2g3=v5;
    gains2g3=v6;
    lfogains2g3=v7;
}
function s2g3par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs2g3=v1;
    lfopitchs2g3=v2;
    pans2g3=v3;
    lfopans2g3=v4;
    lps2g3=v5;
    lfolps2g3=v6;
    hps2g3=v7;
    lfohps2g3=v8;
}
function s2g3del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s2g3=v1;
    del_time_s2g3=v2;
    lfodel_time_s2g3=v3;
    del_feedback_s2g3=v4;
    lfodel_feedback_s2g3=v5;
    del_drywet_s2g3=v6;
    lfodel_drywet_s2g3=v7;
}
function s2g3flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s2g3=v1;
    flang_feedback_s2g3=v2;
    lfoflang_feedback_s2g3=v3;
    flang_rate_s2g3=v4;
    lfoflang_rate_s2g3=v5;
    flang_drywet_s2g3=v6;
    lfoflang_drywet_s2g3=v7;
}
function s2g3rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s2g3=v1;
    rev_decay_s2g3=v2;
    lforev_decay_s2g3=v3;
    rev_drywet_s2g3=v4;
    lforev_drywet_s2g3=v5;
}
var mutes2g4;
var envelopes2g4;
var reverses2g4;
var togglelps2g4;
var togglehps2g4;
var gains2g4;
var lfogains2g4;
var pitchs2g4;
var lfopitchs2g4;
var pans2g4;
var lfopans2g4;
var lps2g4;
var lfolps2g4;
var hps2g4;
var lfohps2g4;
var del_time_s2g4
var del_toggle_s2g4
var lfodel_time_s2g4;
var del_feedback_s2g4;
var lfodel_feedback_s2g4;
var del_drywet_s2g4;
var lfodel_drywet_s2g4;
var flang_toggle_s2g4;
var flang_feedback_s2g4;
var lfoflang_feedback_s2g4;
var flang_rate_s2g4;
var lfoflang_rate_s2g4;
var flang_drywet_s2g4;
var lfoflang_drywet_s2g4;
var rev_toggle_s2g4;
var rev_decay_s2g4;
var lforev_decay_s2g4;
var rev_drywet_s2g4;
var lforev_drywet_s2g4;
function s2g4par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes2g4=v1;
    envelopes2g4=v2;
    reverses2g4=v3;
    togglelps2g4=v4;
    togglehps2g4=v5;
    gains2g4=v6;
    lfogains2g4=v7;
}
function s2g4par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs2g4=v1;
    lfopitchs2g4=v2;
    pans2g4=v3;
    lfopans2g4=v4;
    lps2g4=v5;
    lfolps2g4=v6;
    hps2g4=v7;
    lfohps2g4=v8;
}
function s2g4del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s2g4=v1;
    del_time_s2g4=v2;
    lfodel_time_s2g4=v3;
    del_feedback_s2g4=v4;
    lfodel_feedback_s2g4=v5;
    del_drywet_s2g4=v6;
    lfodel_drywet_s2g4=v7;
}
function s2g4flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s2g4=v1;
    flang_feedback_s2g4=v2;
    lfoflang_feedback_s2g4=v3;
    flang_rate_s2g4=v4;
    lfoflang_rate_s2g4=v5;
    flang_drywet_s2g4=v6;
    lfoflang_drywet_s2g4=v7;
}
function s2g4rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s2g4=v1;
    rev_decay_s2g4=v2;
    lforev_decay_s2g4=v3;
    rev_drywet_s2g4=v4;
    lforev_drywet_s2g4=v5;
}
var mutes3g1;
var envelopes3g1;
var reverses3g1;
var togglelps3g1;
var togglehps3g1;
var gains3g1;
var lfogains3g1;
var pitchs3g1;
var lfopitchs3g1;
var pans3g1;
var lfopans3g1;
var lps3g1;
var lfolps3g1;
var hps3g1;
var lfohps3g1;
var del_time_s3g1
var del_toggle_s3g1
var lfodel_time_s3g1;
var del_feedback_s3g1;
var lfodel_feedback_s3g1;
var del_drywet_s3g1;
var lfodel_drywet_s3g1;
var flang_toggle_s3g1;
var flang_feedback_s3g1;
var lfoflang_feedback_s3g1;
var flang_rate_s3g1;
var lfoflang_rate_s3g1;
var flang_drywet_s3g1;
var lfoflang_drywet_s3g1;
var rev_toggle_s3g1;
var rev_decay_s3g1;
var lforev_decay_s3g1;
var rev_drywet_s3g1;
var lforev_drywet_s3g1;
function s3g1par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes3g1=v1;
    envelopes3g1=v2;
    reverses3g1=v3;
    togglelps3g1=v4;
    togglehps3g1=v5;
    gains3g1=v6;
    lfogains3g1=v7;
}
function s3g1par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs3g1=v1;
    lfopitchs3g1=v2;
    pans3g1=v3;
    lfopans3g1=v4;
    lps3g1=v5;
    lfolps3g1=v6;
    hps3g1=v7;
    lfohps3g1=v8;
}
function s3g1del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s3g1=v1;
    del_time_s3g1=v2;
    lfodel_time_s3g1=v3;
    del_feedback_s3g1=v4;
    lfodel_feedback_s3g1=v5;
    del_drywet_s3g1=v6;
    lfodel_drywet_s3g1=v7;
}
function s3g1flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s3g1=v1;
    flang_feedback_s3g1=v2;
    lfoflang_feedback_s3g1=v3;
    flang_rate_s3g1=v4;
    lfoflang_rate_s3g1=v5;
    flang_drywet_s3g1=v6;
    lfoflang_drywet_s3g1=v7;
}
function s3g1rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s3g1=v1;
    rev_decay_s3g1=v2;
    lforev_decay_s3g1=v3;
    rev_drywet_s3g1=v4;
    lforev_drywet_s3g1=v5;
}
var mutes3g2;
var envelopes3g2;
var reverses3g2;
var togglelps3g2;
var togglehps3g2;
var gains3g2;
var lfogains3g2;
var pitchs3g2;
var lfopitchs3g2;
var pans3g2;
var lfopans3g2;
var lps3g2;
var lfolps3g2;
var hps3g2;
var lfohps3g2;
var del_time_s3g2
var del_toggle_s3g2
var lfodel_time_s3g2;
var del_feedback_s3g2;
var lfodel_feedback_s3g2;
var del_drywet_s3g2;
var lfodel_drywet_s3g2;
var flang_toggle_s3g2;
var flang_feedback_s3g2;
var lfoflang_feedback_s3g2;
var flang_rate_s3g2;
var lfoflang_rate_s3g2;
var flang_drywet_s3g2;
var lfoflang_drywet_s3g2;
var rev_toggle_s3g2;
var rev_decay_s3g2;
var lforev_decay_s3g2;
var rev_drywet_s3g2;
var lforev_drywet_s3g2;
function s3g2par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes3g2=v1;
    envelopes3g2=v2;
    reverses3g2=v3;
    togglelps3g2=v4;
    togglehps3g2=v5;
    gains3g2=v6;
    lfogains3g2=v7;
}
function s3g2par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs3g2=v1;
    lfopitchs3g2=v2;
    pans3g2=v3;
    lfopans3g2=v4;
    lps3g2=v5;
    lfolps3g2=v6;
    hps3g2=v7;
    lfohps3g2=v8;
}
function s3g2del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s3g2=v1;
    del_time_s3g2=v2;
    lfodel_time_s3g2=v3;
    del_feedback_s3g2=v4;
    lfodel_feedback_s3g2=v5;
    del_drywet_s3g2=v6;
    lfodel_drywet_s3g2=v7;
}
function s3g2flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s3g2=v1;
    flang_feedback_s3g2=v2;
    lfoflang_feedback_s3g2=v3;
    flang_rate_s3g2=v4;
    lfoflang_rate_s3g2=v5;
    flang_drywet_s3g2=v6;
    lfoflang_drywet_s3g2=v7;
}
function s3g2rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s3g2=v1;
    rev_decay_s3g2=v2;
    lforev_decay_s3g2=v3;
    rev_drywet_s3g2=v4;
    lforev_drywet_s3g2=v5;
}
var mutes3g3;
var envelopes3g3;
var reverses3g3;
var togglelps3g3;
var togglehps3g3;
var gains3g3;
var lfogains3g3;
var pitchs3g3;
var lfopitchs3g3;
var pans3g3;
var lfopans3g3;
var lps3g3;
var lfolps3g3;
var hps3g3;
var lfohps3g3;
var del_time_s3g3
var del_toggle_s3g3
var lfodel_time_s3g3;
var del_feedback_s3g3;
var lfodel_feedback_s3g3;
var del_drywet_s3g3;
var lfodel_drywet_s3g3;
var flang_toggle_s3g3;
var flang_feedback_s3g3;
var lfoflang_feedback_s3g3;
var flang_rate_s3g3;
var lfoflang_rate_s3g3;
var flang_drywet_s3g3;
var lfoflang_drywet_s3g3;
var rev_toggle_s3g3;
var rev_decay_s3g3;
var lforev_decay_s3g3;
var rev_drywet_s3g3;
var lforev_drywet_s3g3;
function s3g3par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes3g3=v1;
    envelopes3g3=v2;
    reverses3g3=v3;
    togglelps3g3=v4;
    togglehps3g3=v5;
    gains3g3=v6;
    lfogains3g3=v7;
}
function s3g3par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs3g3=v1;
    lfopitchs3g3=v2;
    pans3g3=v3;
    lfopans3g3=v4;
    lps3g3=v5;
    lfolps3g3=v6;
    hps3g3=v7;
    lfohps3g3=v8;
}
function s3g3del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s3g3=v1;
    del_time_s3g3=v2;
    lfodel_time_s3g3=v3;
    del_feedback_s3g3=v4;
    lfodel_feedback_s3g3=v5;
    del_drywet_s3g3=v6;
    lfodel_drywet_s3g3=v7;
}
function s3g3flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s3g3=v1;
    flang_feedback_s3g3=v2;
    lfoflang_feedback_s3g3=v3;
    flang_rate_s3g3=v4;
    lfoflang_rate_s3g3=v5;
    flang_drywet_s3g3=v6;
    lfoflang_drywet_s3g3=v7;
}
function s3g3rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s3g3=v1;
    rev_decay_s3g3=v2;
    lforev_decay_s3g3=v3;
    rev_drywet_s3g3=v4;
    lforev_drywet_s3g3=v5;
}
var mutes3g4;
var envelopes3g4;
var reverses3g4;
var togglelps3g4;
var togglehps3g4;
var gains3g4;
var lfogains3g4;
var pitchs3g4;
var lfopitchs3g4;
var pans3g4;
var lfopans3g4;
var lps3g4;
var lfolps3g4;
var hps3g4;
var lfohps3g4;
var del_time_s3g4
var del_toggle_s3g4
var lfodel_time_s3g4;
var del_feedback_s3g4;
var lfodel_feedback_s3g4;
var del_drywet_s3g4;
var lfodel_drywet_s3g4;
var flang_toggle_s3g4;
var flang_feedback_s3g4;
var lfoflang_feedback_s3g4;
var flang_rate_s3g4;
var lfoflang_rate_s3g4;
var flang_drywet_s3g4;
var lfoflang_drywet_s3g4;
var rev_toggle_s3g4;
var rev_decay_s3g4;
var lforev_decay_s3g4;
var rev_drywet_s3g4;
var lforev_drywet_s3g4;
function s3g4par1(v1,v2,v3,v4,v5,v6,v7)
{
    mutes3g4=v1;
    envelopes3g4=v2;
    reverses3g4=v3;
    togglelps3g4=v4;
    togglehps3g4=v5;
    gains3g4=v6;
    lfogains3g4=v7;
}
function s3g4par2(v1,v2,v3,v4,v5,v6,v7,v8)
{
    pitchs3g4=v1;
    lfopitchs3g4=v2;
    pans3g4=v3;
    lfopans3g4=v4;
    lps3g4=v5;
    lfolps3g4=v6;
    hps3g4=v7;
    lfohps3g4=v8;
}
function s3g4del(v1,v2,v3,v4,v5,v6,v7)
{
    del_toggle_s3g4=v1;
    del_time_s3g4=v2;
    lfodel_time_s3g4=v3;
    del_feedback_s3g4=v4;
    lfodel_feedback_s3g4=v5;
    del_drywet_s3g4=v6;
    lfodel_drywet_s3g4=v7;
}
function s3g4flang(v1,v2,v3,v4,v5,v6,v7)
{
    flang_toggle_s3g4=v1;
    flang_feedback_s3g4=v2;
    lfoflang_feedback_s3g4=v3;
    flang_rate_s3g4=v4;
    lfoflang_rate_s3g4=v5;
    flang_drywet_s3g4=v6;
    lfoflang_drywet_s3g4=v7;
}
function s3g4rev(v1,v2,v3,v4,v5)
{
    rev_toggle_s3g4=v1;
    rev_decay_s3g4=v2;
    lforev_decay_s3g4=v3;
    rev_drywet_s3g4=v4;
    lforev_drywet_s3g4=v5;
}
var lfo1f;
var lfo1a;
var lfo1w;
var lfo2f;
var lfo2a;
var lfo2w;
var lfo3f;
var lfo3a;
var lfo3w;
function savelfo(v1,v2,v3,v4,v5,v6,v7,v8,v9)
{
lfo1f=v1;
lfo1a=v2;
lfo1w=v3;
lfo2f=v4;
lfo2a=v5;
lfo2w=v6;
lfo3f=v7;
lfo3a=v8;
lfo3w=v9;
}


function save()
{
    clear();
    //anything(0, starts1g1, ends1g1, starts1g2, ends1g2, starts1g3, ends1g3, starts1g4, ends1g4, starts2g1, ends2g1, starts2g2, ends2g2, starts2g3, ends2g3, starts2g4, ends2g4, starts3g1, ends3g1, starts3g2, ends3g2, starts3g3, ends3g3, starts3g4, ends3g4, pianoroll,  bpm, sample1load, sample2load, sample3load);

    anything(1, starts1g3);
    anything(0, starts1g1);
    anything(1,ends1g1);
    anything(2, starts1g2);
    anything(3,ends1g2);
    anything(4,starts1g3);
    anything(5,ends1g3);
    anything(6, starts1g4);
    anything(7,ends1g4);
    anything(8, starts2g1);
    anything(9,ends2g1);
    anything(10, starts2g2);
    anything(11,ends2g2);
    anything(12, starts2g3);
    anything(13,ends2g3);
    anything(14, starts2g4);
    anything(15,ends2g4);
    anything(16, starts3g1);
    anything(17,ends3g1);
    anything(18, starts3g2);
    anything(19,ends3g2);
    anything(20, starts3g3);
    anything(21,ends3g3);
    anything(22, starts3g4);
    anything(23,ends3g4);
    anything(24, pianoroll);
    anything(25, bpm);
    anything(26, s1g1b1);
    anything(27, s1g1b2);
    anything(28, s1g1b3);
    anything(29, s1g1b4);
    anything(30, s1g1b5);
    anything(31, s1g1b6);
    anything(32, s1g1b7);
    anything(33, s1g1b8);
    anything(34, s1g1b9);
    anything(35, s1g1b10);
    anything(36, s1g1b11);
    anything(37, s1g1b12);
    anything(38, s1g1b13);
    anything(39, s1g1b14);
    anything(40, s1g1b15);
    anything(41, s1g1b16);
    anything(42, s1g2b1);
    anything(43, s1g2b2);
    anything(44, s1g2b3);
    anything(45, s1g2b4);
    anything(46, s1g2b5);
    anything(47, s1g2b6);
    anything(48, s1g2b7);
    anything(49, s1g2b8);
    anything(50, s1g2b9);
    anything(51, s1g2b10);
    anything(52, s1g2b11);
    anything(53, s1g2b12);
    anything(54, s1g2b13);
    anything(55, s1g2b14);
    anything(56, s1g2b15);
    anything(57, s1g2b16);
    anything(58, s1g3b1);
    anything(59, s1g3b2);
    anything(60, s1g3b3);
    anything(61, s1g3b4);
    anything(62, s1g3b5);
    anything(63, s1g3b6);
    anything(64, s1g3b7);
    anything(65, s1g3b8);
    anything(66, s1g3b9);
    anything(67, s1g3b10);
    anything(68, s1g3b11);
    anything(69, s1g3b12);
    anything(70, s1g3b13);
    anything(71, s1g3b14);
    anything(72, s1g3b15);
    anything(73, s1g3b16);
    anything(74, s1g4b1);
    anything(75, s1g4b2);
    anything(76, s1g4b3);
    anything(77, s1g4b4);
    anything(78, s1g4b5);
    anything(79, s1g4b6);
    anything(80, s1g4b7);
    anything(81, s1g4b8);
    anything(82, s1g4b9);
    anything(83, s1g4b10);
    anything(84, s1g4b11);
    anything(85, s1g4b12);
    anything(86, s1g4b13);
    anything(87, s1g4b14);
    anything(88, s1g4b15);
    anything(89, s1g4b16);
    anything(90, s2g1b1);
    anything(91, s2g1b2);
    anything(92, s2g1b3);
    anything(93, s2g1b4);
    anything(94, s2g1b5);
    anything(95, s2g1b6);
    anything(96, s2g1b7);
    anything(97, s2g1b8);
    anything(98, s2g1b9);
    anything(99, s2g1b10);
    anything(100, s2g1b11);
    anything(101, s2g1b12);
    anything(102, s2g1b13);
    anything(103, s2g1b14);
    anything(104, s2g1b15);
    anything(105, s2g1b16);
    anything(106, s2g2b1);
    anything(107, s2g2b2);
    anything(108, s2g2b3);
    anything(109, s2g2b4);
    anything(110, s2g2b5);
    anything(111, s2g2b6);
    anything(112, s2g2b7);
    anything(113, s2g2b8);
    anything(114, s2g2b9);
    anything(115, s2g2b10);
    anything(116, s2g2b11);
    anything(117, s2g2b12);
    anything(118, s2g2b13);
    anything(119, s2g2b14);
    anything(120, s2g2b15);
    anything(121, s2g2b16);
    anything(122, s2g3b1);
    anything(123, s2g3b2);
    anything(124, s2g3b3);
    anything(125, s2g3b4);
    anything(126, s2g3b5);
    anything(127, s2g3b6);
    anything(128, s2g3b7);
    anything(129, s2g3b8);
    anything(130, s2g3b9);
    anything(131, s2g3b10);
    anything(132, s2g3b11);
    anything(133, s2g3b12);
    anything(134, s2g3b13);
    anything(135, s2g3b14);
    anything(136, s2g3b15);
    anything(137, s2g3b16);
    anything(138, s2g4b1);
    anything(139, s2g4b2);
    anything(140, s2g4b3);
    anything(141, s2g4b4);
    anything(142, s2g4b5);
    anything(143, s2g4b6);
    anything(144, s2g4b7);
    anything(145, s2g4b8);
    anything(146, s2g4b9);
    anything(147, s2g4b10);
    anything(148, s2g4b11);
    anything(149, s2g4b12);
    anything(150, s2g4b13);
    anything(151, s2g4b14);
    anything(152, s2g4b15);
    anything(153, s2g4b16);
    anything(154, s3g1b1);
    anything(155, s3g1b2);
    anything(156, s3g1b3);
    anything(157, s3g1b4);
    anything(158, s3g1b5);
    anything(159, s3g1b6);
    anything(160, s3g1b7);
    anything(161, s3g1b8);
    anything(162, s3g1b9);
    anything(163, s3g1b10);
    anything(164, s3g1b11);
    anything(165, s3g1b12);
    anything(166, s3g1b13);
    anything(167, s3g1b14);
    anything(168, s3g1b15);
    anything(169, s3g1b16);
    anything(170, s3g2b1);
    anything(171, s3g2b2);
    anything(172, s3g2b3);
    anything(173, s3g2b4);
    anything(174, s3g2b5);
    anything(175, s3g2b6);
    anything(176, s3g2b7);
    anything(177, s3g2b8);
    anything(178, s3g2b9);
    anything(179, s3g2b10);
    anything(180, s3g2b11);
    anything(181, s3g2b12);
    anything(182, s3g2b13);
    anything(183, s3g2b14);
    anything(184, s3g2b15);
    anything(185, s3g2b16);
    anything(186, s3g3b1);
    anything(187, s3g3b2);
    anything(188, s3g3b3);
    anything(189, s3g3b4);
    anything(190, s3g3b5);
    anything(191, s3g3b6);
    anything(192, s3g3b7);
    anything(193, s3g3b8);
    anything(194, s3g3b9);
    anything(195, s3g3b10);
    anything(196, s3g3b11);
    anything(197, s3g3b12);
    anything(198, s3g3b13);
    anything(199, s3g3b14);
    anything(200, s3g3b15);
    anything(201, s3g3b16);
    anything(202, s3g4b1);
    anything(203, s3g4b2);
    anything(204, s3g4b3);
    anything(205, s3g4b4);
    anything(206, s3g4b5);
    anything(207, s3g4b6);
    anything(208, s3g4b7);
    anything(209, s3g4b8);
    anything(210, s3g4b9);
    anything(211, s3g4b10);
    anything(212, s3g4b11);
    anything(213, s3g4b12);
    anything(214, s3g4b13);
    anything(215, s3g4b14);
    anything(216, s3g4b15);
    anything(217, s3g4b16);
    anything(218, mutes1g1);
    anything(219, envelopes1g1);
    anything(220, reverses1g1);
    anything(221, togglelps1g1);
    anything(222, togglehps1g1);
    anything(223, gains1g1);
    anything(224, lfogains1g1);
    anything(225, pitchs1g1);
    anything(226, lfopitchs1g1);
    anything(227, pans1g1);
    anything(229, lfopans1g1);
    anything(229, lps1g1);
    anything(230, lfolps1g1);
    anything(231, hps1g1);
    anything(232, lfohps1g1);
    anything(233, del_time_s1g1);
    anything(234, del_toggle_s1g1);
    anything(235, lfodel_time_s1g1);
    anything(236, del_feedback_s1g1);
    anything(237, lfodel_feedback_s1g1);
    anything(238, del_drywet_s1g1);
    anything(239, lfodel_drywet_s1g1);
    anything(240, flang_toggle_s1g1);
    anything(241, flang_feedback_s1g1);
    anything(242, lfoflang_feedback_s1g1);
    anything(243, flang_rate_s1g1);
    anything(244, lfoflang_rate_s1g1);
    anything(245, flang_drywet_s1g1);
    anything(246, lfoflang_drywet_s1g1);
    anything(247, rev_toggle_s1g1);
    anything(248, rev_decay_s1g1);
    anything(249, lforev_decay_s1g1);
    anything(250, rev_drywet_s1g1);
    anything(251, lforev_drywet_s1g1);

    anything(252, mutes1g2);
    anything(253, envelopes1g2);
    anything(254, reverses1g2);
    anything(255, togglelps1g2);
    anything(256, togglehps1g2);
    anything(257, gains1g2);
    anything(258, lfogains1g2);
    anything(259, pitchs1g2);
    anything(260, lfopitchs1g2);
    anything(261, pans1g2);
    anything(262, lfopans1g2);
    anything(263, lps1g2);
    anything(264, lfolps1g2);
    anything(265, hps1g2);
    anything(266, lfohps1g2);
    anything(267, del_time_s1g2);
    anything(268, del_toggle_s1g2);
    anything(269, lfodel_time_s1g2);
    anything(270, del_feedback_s1g2);
    anything(271, lfodel_feedback_s1g2);
    anything(272, del_drywet_s1g2);
    anything(273, lfodel_drywet_s1g2);
    anything(274, flang_toggle_s1g2);
    anything(275, flang_feedback_s1g2);
    anything(276, lfoflang_feedback_s1g2);
    anything(277, flang_rate_s1g2);
    anything(278, lfoflang_rate_s1g2);
    anything(279, flang_drywet_s1g2);
    anything(280, lfoflang_drywet_s1g2);
    anything(281, rev_toggle_s1g2);
    anything(282, rev_decay_s1g2);
    anything(283, lforev_decay_s1g2);
    anything(284, rev_drywet_s1g2);
    anything(285, lforev_drywet_s1g2);

    anything(286, mutes1g3);
    anything(287, envelopes1g3);
    anything(288, reverses1g3);
    anything(289, togglelps1g3);
    anything(290, togglehps1g3);
    anything(291, gains1g3);
    anything(292, lfogains1g3);
    anything(293, pitchs1g3);
    anything(294, lfopitchs1g3);
    anything(295, pans1g3);
    anything(296, lfopans1g3);
    anything(297, lps1g3);
    anything(298, lfolps1g3);
    anything(299, hps1g3);
    anything(300, lfohps1g3);
    anything(301, del_time_s1g3);
    anything(302, del_toggle_s1g3);
    anything(303, lfodel_time_s1g3);
    anything(304, del_feedback_s1g3);
    anything(305, lfodel_feedback_s1g3);
    anything(306, del_drywet_s1g3);
    anything(307, lfodel_drywet_s1g3);
    anything(308, flang_toggle_s1g3);
    anything(309, flang_feedback_s1g3);
    anything(310, lfoflang_feedback_s1g3);
    anything(311, flang_rate_s1g3);
    anything(312, lfoflang_rate_s1g3);
    anything(313, flang_drywet_s1g3);
    anything(314, lfoflang_drywet_s1g3);
    anything(315, rev_toggle_s1g3);
    anything(316, rev_decay_s1g3);
    anything(317, lforev_decay_s1g3);
    anything(318, rev_drywet_s1g3);
    anything(319, lforev_drywet_s1g3);
    anything(320, mutes1g4);
    anything(321, envelopes1g4);
    anything(322, reverses1g4);
    anything(323, togglelps1g4);
    anything(324, togglehps1g4);
    anything(325, gains1g4);
    anything(326, lfogains1g4);
    anything(327, pitchs1g4);
    anything(328, lfopitchs1g4);
    anything(329, pans1g4);
    anything(330, lfopans1g4);
    anything(331, lps1g4);
    anything(332, lfolps1g4);
    anything(333, hps1g4);
    anything(334, lfohps1g4);
    anything(335, del_time_s1g4);
    anything(336, del_toggle_s1g4);
    anything(337, lfodel_time_s1g4);
    anything(338, del_feedback_s1g4);
    anything(339, lfodel_feedback_s1g4);
    anything(340, del_drywet_s1g4);
    anything(341, lfodel_drywet_s1g4);
    anything(342, flang_toggle_s1g4);
    anything(343, flang_feedback_s1g4);
    anything(344, lfoflang_feedback_s1g4);
    anything(345, flang_rate_s1g4);
    anything(346, lfoflang_rate_s1g4);
    anything(347, flang_drywet_s1g4);
    anything(348, lfoflang_drywet_s1g4);
    anything(349, rev_toggle_s1g4);
    anything(350, rev_decay_s1g4);
    anything(351, lforev_decay_s1g4);
    anything(352, rev_drywet_s1g4);
    anything(353, lforev_drywet_s1g4);

    anything(354, mutes2g1);
    anything(355, envelopes2g1);
    anything(356, reverses2g1);
    anything(357, togglelps2g1);
    anything(358, togglehps2g1);
    anything(359, gains2g1);
    anything(360, lfogains2g1);
    anything(361, pitchs2g1);
    anything(362, lfopitchs2g1);
    anything(363, pans2g1);
    anything(364, lfopans2g1);
    anything(365, lps2g1);
    anything(366, lfolps2g1);
    anything(367, hps2g1);
    anything(368, lfohps2g1);
    anything(369, del_time_s2g1);
    anything(370, del_toggle_s2g1);
    anything(371, lfodel_time_s2g1);
    anything(372, del_feedback_s2g1);
    anything(373, lfodel_feedback_s2g1);
    anything(374, del_drywet_s2g1);
    anything(375, lfodel_drywet_s2g1);
    anything(376, flang_toggle_s2g1);
    anything(377, flang_feedback_s2g1);
    anything(378, lfoflang_feedback_s2g1);
    anything(379, flang_rate_s2g1);
    anything(380, lfoflang_rate_s2g1);
    anything(381, flang_drywet_s2g1);
    anything(382, lfoflang_drywet_s2g1);
    anything(383, rev_toggle_s2g1);
    anything(384, rev_decay_s2g1);
    anything(385, lforev_decay_s2g1);
    anything(386, rev_drywet_s2g1);
    anything(387, lforev_drywet_s2g1);
    anything(388, mutes2g2);
    anything(389, envelopes2g2);
    anything(390, reverses2g2);
    anything(391, togglelps2g2);
    anything(392, togglehps2g2);
    anything(393, gains2g2);
    anything(394, lfogains2g2);
    anything(395, pitchs2g2);
    anything(396, lfopitchs2g2);
    anything(397, pans2g2);
    anything(398, lfopans2g2);
    anything(399, lps2g2);
    anything(400, lfolps2g2);
    anything(401, hps2g2);
    anything(402, lfohps2g2);
    anything(403, del_time_s2g2);
    anything(404, del_toggle_s2g2);
    anything(405, lfodel_time_s2g2);
    anything(406, del_feedback_s2g2);
    anything(407, lfodel_feedback_s2g2);
    anything(408, del_drywet_s2g2);
    anything(409, lfodel_drywet_s2g2);
    anything(410, flang_toggle_s2g2);
    anything(411, flang_feedback_s2g2);
    anything(412, lfoflang_feedback_s2g2);
    anything(413, flang_rate_s2g2);
    anything(414, lfoflang_rate_s2g2);
    anything(415, flang_drywet_s2g2);
    anything(416, lfoflang_drywet_s2g2);
    anything(417, rev_toggle_s2g2);
    anything(418, rev_decay_s2g2);
    anything(419, lforev_decay_s2g2);
    anything(420, rev_drywet_s2g2);
    anything(421, lforev_drywet_s2g2)
    anything(422, mutes2g3);
    anything(423, envelopes2g3);
    anything(424, reverses2g3);
    anything(425, togglelps2g3);
    anything(426, togglehps2g3);
    anything(427, gains2g3);
    anything(428, lfogains2g3);
    anything(429, pitchs2g3);
    anything(430, lfopitchs2g3);
    anything(431, pans2g3);
    anything(432, lfopans2g3);
    anything(433, lps2g3);
    anything(434, lfolps2g3);
    anything(435, hps2g3);
    anything(436, lfohps2g3);
    anything(437, del_time_s2g3);
    anything(438, del_toggle_s2g3);
    anything(439, lfodel_time_s2g3);
    anything(440, del_feedback_s2g3);
    anything(441, lfodel_feedback_s2g3);
    anything(442, del_drywet_s2g3);
    anything(443, lfodel_drywet_s2g3);
    anything(444, flang_toggle_s2g3);
    anything(445, flang_feedback_s2g3);
    anything(446, lfoflang_feedback_s2g3);
    anything(447, flang_rate_s2g3);
    anything(448, lfoflang_rate_s2g3);
    anything(449, flang_drywet_s2g3);
    anything(450, lfoflang_drywet_s2g3);
    anything(451, rev_toggle_s2g3);
    anything(452, rev_decay_s2g3);
    anything(453, lforev_decay_s2g3);
    anything(454, rev_drywet_s2g3);
    anything(455, lforev_drywet_s2g3);
    anything(456, mutes2g4);
    anything(457, envelopes2g4);
    anything(458, reverses2g4);
    anything(459, togglelps2g4);
    anything(460, togglehps2g4);
    anything(461, gains2g4);
    anything(462, lfogains2g4);
    anything(463, pitchs2g4);
    anything(464, lfopitchs2g4);
    anything(465, pans2g4);
    anything(466, lfopans2g4);
    anything(467, lps2g4);
    anything(468, lfolps2g4);
    anything(469, hps2g4);
    anything(470, lfohps2g4);
    anything(471, del_time_s2g4);
    anything(472, del_toggle_s2g4);
    anything(473, lfodel_time_s2g4);
    anything(474, del_feedback_s2g4);
    anything(475, lfodel_feedback_s2g4);
    anything(476, del_drywet_s2g4);
    anything(477, lfodel_drywet_s2g4);
    anything(478, flang_toggle_s2g4);
    anything(479, flang_feedback_s2g4);
    anything(480, lfoflang_feedback_s2g4);
    anything(481, flang_rate_s2g4);
    anything(482, lfoflang_rate_s2g4);
    anything(483, flang_drywet_s2g4);
    anything(484, lfoflang_drywet_s2g4);
    anything(485, rev_toggle_s2g4);
    anything(486, rev_decay_s2g4);
    anything(487, lforev_decay_s2g4);
    anything(488, rev_drywet_s2g4);
    anything(489, lforev_drywet_s2g4);

    anything(490, mutes3g1);
    anything(491, envelopes3g1);
    anything(492, reverses3g1);
    anything(493, togglelps3g1);
    anything(494, togglehps3g1);
    anything(495, gains3g1);
    anything(496, lfogains3g1);
    anything(497, pitchs3g1);
    anything(498, lfopitchs3g1);
    anything(499, pans3g1);
    anything(500, lfopans3g1);
    anything(501, lps3g1);
    anything(502, lfolps3g1);
    anything(503, hps3g1);
    anything(504, lfohps3g1);
    anything(505, del_time_s3g1);
    anything(506, del_toggle_s3g1);
    anything(507, lfodel_time_s3g1);
    anything(508, del_feedback_s3g1);
    anything(509, lfodel_feedback_s3g1);
    anything(510, del_drywet_s3g1);
    anything(511, lfodel_drywet_s3g1);
    anything(512, flang_toggle_s3g1);
    anything(513, flang_feedback_s3g1);
    anything(514, lfoflang_feedback_s3g1);
    anything(515, flang_rate_s3g1);
    anything(516, lfoflang_rate_s3g1);
    anything(517, flang_drywet_s3g1);
    anything(518, lfoflang_drywet_s3g1);
    anything(519, rev_toggle_s3g1);
    anything(520, rev_decay_s3g1);
    anything(521, lforev_decay_s3g1);
    anything(522, rev_drywet_s3g1);
    anything(523, lforev_drywet_s3g1);
    anything(524, mutes3g2);
    anything(525, envelopes3g2);
    anything(526, reverses3g2);
    anything(527, togglelps3g2);
    anything(528, togglehps3g2);
    anything(529, gains3g2);
    anything(530, lfogains3g2);
    anything(531, pitchs3g2);
    anything(532, lfopitchs3g2);
    anything(533, pans3g2);
    anything(534, lfopans3g2);
    anything(535, lps3g2);
    anything(536, lfolps3g2);
    anything(537, hps3g2);
    anything(538, lfohps3g2);
    anything(539, del_time_s3g2);
    anything(540, del_toggle_s3g2);
    anything(541, lfodel_time_s3g2);
    anything(542, del_feedback_s3g2);
    anything(543, lfodel_feedback_s3g2);
    anything(544, del_drywet_s3g2);
    anything(545, lfodel_drywet_s3g2);
    anything(546, flang_toggle_s3g2);
    anything(547, flang_feedback_s3g2);
    anything(548, lfoflang_feedback_s3g2);
    anything(549, flang_rate_s3g2);
    anything(550, lfoflang_rate_s3g2);
    anything(551, flang_drywet_s3g2);
    anything(552, lfoflang_drywet_s3g2);
    anything(553, rev_toggle_s3g2);
    anything(554, rev_decay_s3g2);
    anything(555, lforev_decay_s3g2);
    anything(556, rev_drywet_s3g2);
    anything(557, lforev_drywet_s3g2)
    anything(558, mutes3g3);
    anything(559, envelopes3g3);
    anything(560, reverses3g3);
    anything(561, togglelps3g3);
    anything(562, togglehps3g3);
    anything(563, gains3g3);
    anything(564, lfogains3g3);
    anything(565, pitchs3g3);
    anything(566, lfopitchs3g3);
    anything(567, pans3g3);
    anything(568, lfopans3g3);
    anything(569, lps3g3);
    anything(570, lfolps3g3);
    anything(571, hps3g3);
    anything(572, lfohps3g3);
    anything(573, del_time_s3g3);
    anything(574, del_toggle_s3g3);
    anything(575, lfodel_time_s3g3);
    anything(576, del_feedback_s3g3);
    anything(577, lfodel_feedback_s3g3);
    anything(578, del_drywet_s3g3);
    anything(579, lfodel_drywet_s3g3);
    anything(580, flang_toggle_s3g3);
    anything(581, flang_feedback_s3g3);
    anything(582, lfoflang_feedback_s3g3);
    anything(583, flang_rate_s3g3);
    anything(584, lfoflang_rate_s3g3);
    anything(585, flang_drywet_s3g3);
    anything(586, lfoflang_drywet_s3g3);
    anything(587, rev_toggle_s3g3);
    anything(588, rev_decay_s3g3);
    anything(589, lforev_decay_s3g3);
    anything(590, rev_drywet_s3g3);
    anything(591, lforev_drywet_s3g3);
    anything(592, mutes3g4);
    anything(593, envelopes3g4);
    anything(594, reverses3g4);
    anything(595, togglelps3g4);
    anything(596, togglehps3g4);
    anything(597, gains3g4);
    anything(598, lfogains3g4);
    anything(599, pitchs3g4);
    anything(600, lfopitchs3g4);
    anything(601, pans3g4);
    anything(602, lfopans3g4);
    anything(603, lps3g4);
    anything(604, lfolps3g4);
    anything(605, hps3g4);
    anything(606, lfohps3g4);
    anything(607, del_time_s3g4);
    anything(608, del_toggle_s3g4);
    anything(609, lfodel_time_s3g4);
    anything(610, del_feedback_s3g4);
    anything(611, lfodel_feedback_s3g4);
    anything(612, del_drywet_s3g4);
    anything(613, lfodel_drywet_s3g4);
    anything(614, flang_toggle_s3g4);
    anything(615, flang_feedback_s3g4);
    anything(616, lfoflang_feedback_s3g4);
    anything(617, flang_rate_s3g4);
    anything(618, lfoflang_rate_s3g4);
    anything(619, flang_drywet_s3g4);
    anything(620, lfoflang_drywet_s3g4);
    anything(621, rev_toggle_s3g4);
    anything(622, rev_decay_s3g4);
    anything(623, lforev_decay_s3g4);
    anything(624, rev_drywet_s3g4);
    anything(625, lforev_drywet_s3g4);
	anything(626, lfo1f);
	anything(627, lfo1a);
	anything(628, lfo1w);
	anything(629, lfo2f);
	anything(630, lfo2a);
	anything(631, lfo2w);
	anything(632, lfo3f);
	anything(633, lfo3a);
	anything(634, lfo3w);
    
	anything(635, sample1name);
	anything(636, sample2name);
	anything(637, sample3name); 
	
}
