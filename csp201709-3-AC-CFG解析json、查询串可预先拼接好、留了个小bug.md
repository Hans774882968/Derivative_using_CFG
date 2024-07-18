// 更新样例：字符串里几乎可以是任意特殊字符

```json
7 5
{":\":123{\"{": {
    "456": ",\",}\"}:\":\"",
    "789": "no :: surprise",
    "\"\\123\\\"": "\\{\\{\\}\\}\\",
    "json": "\"address\":{\"streetAddress\":\"2ndStreet\",\"\\\\\":\"\\\\\"}"
}
}
:":123{"{
:":123{"{.456
:":123{"{.789
:":123{"{."\123\"
:":123{"{.json
//OBJECT
STRING ,",}"}:":"
STRING no :: surprise
STRING \{\{\}\}\
STRING "address":{"streetAddress":"2ndStreet","\\":"\\"}

16 15
{
    "firstName": "John",
    "lastName": "Smith",
    "address": {
        "streetAddress": "2ndStreet",
        "city": "NewYork",
        "state": "NY",
        "\"": "\\\\\\\\",
        "\\\\\\\\" : {
            "esc\\aped":"\"\"hello\"\"",
            "\\||":"||\\",
            "\\\"": "\"\\"
        }
    },
    "esc\\aped": "\"hello\""
}
esc\aped
address."
address.\\\\
address.\\\\.esc\aped
address.\\\\.\||
address.\\\\.\"
//STRING "hello"
STRING \\\\
OBJECT
STRING ""hello""
STRING ||\
STRING "\

8 5
{
    "233n": "234",
    "obj": {
        "val": ", :: ,\",\"",
        "ss": "sss"
    },
    "\\":    "\""
}
233n
obj
obj.val
obj.ss
\
//STRING 234
OBJECT
STRING , :: ,","
STRING sss
STRING "

14 10
{
"firstName": "John",
"lastName": "Smith",
"address": {
    "streetAddress": "2ndStreet",
    "city": "NewYork",
    "state": "NY",
    "fucking": {
        "xx": "yy",
        "xxx": "\""
    }
},
    "esc\\aped": "\"hello\""
}
firstName
address
address.city
address.postal
esc\aped
firstName
address.fucking.xxx
address.fucking
address.fucking.xx
//STRING John
OBJECT
STRING NewYork
NOTEXIST
STRING "hello"
STRING John
STRING "
OBJECT
STRING yy
```

ps：虽然1发AC，但是当时图省事自己实现的`replaceAll`居然是有bug的……当然现在已经改正了。另外，这个题我的代码有点小bug，在csp的测试数据里有一个样例会`assert`失败（虽然去掉`assert`也能AC）。暂时不知道原因了。

思路就是`parse`，并把键值对写入`map<string, string>`。然后处理查询有个小技巧，就是在`parse`的时候顺便拼接好所有的点，在递归的时候带上prefixes参数。这样就可以直接在上述全局map查找了。`parse`的套路十分简单，里面若干`assert`也提供了直观的提示。
我认为唯一难的点，就是`advance`的时候要知道字符串的边界在哪里。我查看了若干开源代码，有的是采用我代码里所注释的写法，那个显然是有bug产生的。有的是采用了和我一样的提前看2位，把冒号逗号当作分割符的写法，这个写法也会有bug，比如：`"\":"`和`"\\\\\"\":"`。如果尝试先把转义的反斜杠给去掉，似乎也只会让这个难点更难……
要彻底避开这些有bug的写法，只能先解决一个子问题：每个反斜杠分别和谁匹配。我真的不会这个啊QAQ我好菜啊好蒻啊555。
留作思考题：求大神告诉我发明json的人是怎么判断字符串的边界的QAQ（划掉）

思考题解答：我会了，我又行了！
若碰到一个反斜杠，则状态转换。即“无反斜杠需要匹配”变成“有”，反之同理。若碰到一个引号，如果此时无反斜杠需要匹配，则说明已经到达边界。否则未到达边界，并标记此时已无反斜杠需要匹配。对该算法进行拓展可以支持\n、\r等符号，但为了简化问题，我们还是沿用题目的假设。代码：

```cpp
tmpp = pos;
int slash_state = 0;
for(;pos < s.size();++pos){
    if(s[pos] == '\\')
        slash_state = !slash_state;
    else if(pos > tmpp && s[pos] == '"'){
        if(!slash_state) break;
        else slash_state = 0;
    }
}
```

于是可以给出更加强大的v2（可惜还是有`assert`失败，不管了AC就好了）。它稍作修改即可完成另一个项目：json转xml。