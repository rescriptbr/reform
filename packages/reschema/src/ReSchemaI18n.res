type t = {
  false_: unit => string,
  true_: unit => string,
  intMin: (~value: int, ~min: int) => string,
  intMax: (~value: int, ~max: int) => string,
  floatMin: (~value: float, ~min: float) => string,
  floatMax: (~value: float, ~max: float) => string,
  email: (~value: string) => string,
  stringNonEmpty: (~value: string) => string,
  stringRegExp: (~value: string, ~pattern: string) => string,
  stringMin: (~value: string, ~min: int) => string,
  stringMax: (~value: string, ~max: int) => string,
};

let default = {
  false_: () => "This value should be false",
  true_: () => "This value should be true",
  intMin: (~value as _value, ~min) =>
    "This value must be greater than or equal to " ++ string_of_int(min),
  intMax: (~value as _value, ~max) =>
    "This value must be less than or equal to " ++ string_of_int(max),
  floatMin: (~value as _value, ~min) =>
    "This value must be greater than or equal to " ++ Js.Float.toString(min),
  floatMax: (~value as _value, ~max) =>
    "This value must be less than or equal to " ++ Js.Float.toString(max),
  email: (~value) => value ++ " is not a valid email",
  stringNonEmpty: (~value as _) => "String must not be empty",
  stringRegExp: (~value as _value, ~pattern) =>
    "This value must match the following: /" ++ pattern ++ "/",
  stringMin: (~value as _value, ~min) =>
    "This value must be at least " ++ string_of_int(min) ++ " characters",
  stringMax: (~value as _value, ~max) =>
    "This value must be at most " ++ string_of_int(max) ++ " characters",
};
