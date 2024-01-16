type testable<'a>
type jestFn

@val @scope("jest") external fn0: (unit => 'a) => jestFn = "fn"
@val @scope("jest") external fn1: ('a => 'b) => jestFn = "fn"
@val @scope("jest") external fn2: (('a, 'b) => 'c, 'a, 'b) => 'c = "fn"

@val external describe: (string, (. unit) => unit) => unit = "describe"
@val external it: (string, (. unit) => unit) => unit = "it"
@val external test: (string, (. unit) => unit) => unit = "test"

@val external expect: 'value => testable<'value> = "expect"
@val external expectFn: jestFn => testable<'value> = "expect"
@val external expectArray: array<'value> => testable<'value> = "expect"

@send external toBe: (testable<'value>, 'value) => unit = "toBe"
@send external toEqual: (testable<'value>, 'value) => testable<'value> = "toEqual"
@send external toStrictEqual: (testable<'value>, 'value) => testable<'value> = "toStrictEqual"
@send external toContain: (testable<'value>, 'value) => testable<'value> = "toContain"
@send external toHaveBeenCalled: (testable<'value>, unit) => testable<'value> = "toHaveBeenCalled"
@send
external toHaveBeenCalledTimes: (testable<'value>, int) => testable<'value> =
  "toHaveBeenCalledTimes"

@get external not: testable<'value> => testable<'value> = "not"
