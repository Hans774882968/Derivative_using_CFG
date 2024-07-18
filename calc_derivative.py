import numpy as np
from sympy import *


def format_op(str_f):
    return str_f.replace("+", " + ").replace("-", " - ").replace("*", " * ").replace("^", " ^ ")


class Derivative():
    def __init__(self, s, varname):
        self.i = 0
        self.s = s.replace(" ", "") + "#"
        self.token = ""
        self.varname = varname

    def advance(self):
        if self.i >= len(self.s):
            self.token = None
        else:
            if self.s[self.i].isdigit():
                i0 = self.i
                while self.i < len(self.s) and self.s[self.i].isdigit():
                    self.i += 1
                self.token = self.s[i0:self.i]
            elif self.s[self.i].isalpha():
                i0 = self.i
                while self.i < len(self.s) and self.s[self.i].isalpha():
                    self.i += 1
                self.token = self.s[i0:self.i]
            else:
                self.token = self.s[self.i]
                self.i += 1
        return self.token

    def process_ln(self):
        token = self.advance()
        if token != "(":
            return False, "invalid_func_ln", "invalid_func_ln"
        fl, ans, func = self.derivative()
        if not fl or self.token != ")":
            return False, "invalid_func_ln", "invalid_func_ln"
        self.advance()  # 跳过")"
        ans = "(%s) / (%s)" % (ans, format_op(func))  # TODO: ans = "x"等情况不需要加括号
        func = "ln(%s)" % func
        return True, ans, func

    def process_cos(self):
        token = self.advance()
        if token != "(":
            return False, "invalid_func_cos", "invalid_func_cos"
        fl, ans, func = self.derivative()
        if not fl or self.token != ")":
            return False, "invalid_func_cos", "invalid_func_cos"
        self.advance()  # 跳过")"
        ans = "(-sin(%s)) * (%s)" % (format_op(func), ans)  # TODO: ans = "x"等情况不需要加括号
        func = "cos(%s)" % func
        return True, ans, func

    def process_sin(self):
        token = self.advance()
        if token != "(":
            return False, "invalid_func_sin", "invalid_func_sin"
        fl, ans, func = self.derivative()
        if not fl or self.token != ")":
            return False, "invalid_func_sin", "invalid_func_sin"
        self.advance()  # 跳过")"
        ans = "cos(%s) * (%s)" % (format_op(func), ans)  # TODO: ans = "x"等情况不需要加括号
        func = "sin(%s)" % func
        return True, ans, func

    def process_poly(self, ch):
        token = self.advance()
        ans, func = "1", ch
        if token == "^":
            ans2 = self.advance()
            if not ans2.isdigit():
                return False, "unsupported_func", "unsupported_func"  # 暂不支持指数部分是函数（以括号开头的）以及varname本身
            pn = int(ans2)
            if pn == 1:
                ans = "1"
            elif pn == 0:
                ans = "0"
            elif pn == 2:
                ans = "%s * %s" % (pn, ch)
            else:
                ans = "%s * %s ^ %s" % (pn, ch, pn - 1)
            token = self.advance()  # 一举两得：若是正常的其他运算符，derivative()和T()也要求往后跳
            if token == "^":
                return False, "unsupported_func", "unsupported_func"  # 暂不支持嵌套的求幂
            func = "%s^%s" % (ch, ans2)
        else:
            ans, func = "1", ch
        return True, ans, func

    def process_exp(self, ch):
        token = self.advance()
        ans, func = "0", ch
        if token == "^":
            ans2 = self.advance()
            if ans2.isdigit():
                ans = "0"
            elif ans2 == self.varname:
                if int(ch) == 0:
                    ans = "0"  # 定义0 ^ x的导数为0
                else:
                    ans = "%s ^ %s * ln(%s)" % (ch, ans2, ch)
            else:
                return False, "unsupported_func", "unsupported_func"  # 暂不支持指数部分是函数（以括号开头的）
            token = self.advance()  # 一举两得：若是"^"以外的运算符，derivative()和T()也将要求往后跳
            if token == "^":
                return False, "unsupported_func", "unsupported_func"  # 暂不支持嵌套的求幂
            func = "%s^%s" % (ch, ans2)
        else:
            ans, func = "0", ch
        return True, ans, func

    def E(self):
        ch = self.advance()
        fl, ans, func = True, "", ""
        if ch and ch == "(":
            fl, ans, func = self.derivative()
            if not fl or self.token != ")":
                return False, "unmatched_brackets", "unmatched_brackets"
            self.advance()  # 跳过")"
            ans = "(" + ans + ")"  # TODO: ans = "x"等情况不需要加括号
            func = "(" + func + ")"
        elif ch and ch == "-":
            fl, ans, func = self.E()
            if not fl:
                return False, "invalid_func", "invalid_func"
            ans = "-" + ans
            func = "-" + func
        elif ch and ch.isalpha():
            if ch == "ln":
                fl, ans, func = self.process_ln()
            elif ch == "cos":
                fl, ans, func = self.process_cos()
            elif ch == "sin":
                fl, ans, func = self.process_sin()
            elif ch == self.varname:
                fl, ans, func = self.process_poly(ch)
            else:
                return False, "unknown_string", "unknown_string"
            if not fl:
                return False, ans, func
        elif ch and ch.isdigit():
            fl, ans, func = self.process_exp(ch)
            if not fl:
                return False, ans, func
        else:
            return False, "invalid_func", "invalid_func"
        return True, ans, func
    # parts：存各部分导数
    # 不需要加括号，因为一个因子有加减号的情况下，函数都是自带括号的

    def T(self):
        parts = []
        fl, part0, func0 = self.E()
        if not fl:
            return fl, part0, func0
        parts.append(part0)
        func_parts = [func0]
        while self.token == "*":
            fl2, part2, func2 = self.E()
            fl = fl and fl2
            if not fl:
                return fl, part2, func2
            parts.append(part2)
            func_parts.append(func2)
        func = "*".join(func_parts)
        func_parts = [format_op(fps) for fps in func_parts]
        ans_parts = []
        for j in range(len(parts)):
            tmpp = func_parts[j]
            func_parts[j] = parts[j]
            if parts[j].isdigit() and int(parts[j]) == 0:  # 去除为0的部分
                func_parts[j] = tmpp
                continue
            ans_parts.append(" * ".join(func_parts))
            func_parts[j] = tmpp
        ans = " + ".join(ans_parts)
        return fl, ans, func

    def derivative(self):
        fl, ans, func = self.T()
        if not fl:
            return fl, ans, func
        if len(ans) == 0:
            ans = "0"
        while self.token == "+" or self.token == "-":
            sgn = self.token
            fl2, ans2, func2 = self.T()
            fl = fl and fl2
            if not fl:
                return fl, ans2, func2
            if (sgn == "+" and ans2) or ans2.isalpha() or ans2.isdigit():
                ans += " %s %s" % (sgn, ans2)
            elif ans2:
                ans += " %s (%s)" % (sgn, ans2)
            func += sgn + func2
        return fl, ans, func

    def solve(self):
        fl, ans, func = self.derivative()
        if fl and self.i < len(self.s) - 1:
            return False, "unfinished_read"
        return fl, ans


def duipai(s, x):
    expr = sympify(s)
    res = expr.diff(x)
    print("求导结果：", res)  # dbg
    vals = [res.evalf(subs={x: v, }) for v in np.arange(1.1, 7.6, 0.1)]
    return vals


def main():
    while True:
        s = input()
        varname = input()
        der = Derivative(s, varname)
        fl, ans = der.solve()
        print("表达式是否合法：", fl)
        print(ans)
        if not fl:
            continue
        x = symbols(varname)
        expr = sympify(ans.replace("^", "**"))
        vals = [expr.evalf(subs={x: v, }) for v in np.arange(1.1, 7.6, 0.1)]
        vals2 = duipai(s, x)
        assert vals2 == vals


if __name__ == "__main__":
    main()
