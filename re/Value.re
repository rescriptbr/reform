/* All field values ReForm can handle */
type t =
  | Int(int)
  | Float(float)
  | Bool(bool)
  | String(string)
  | Empty;

let unwrapInt = (value, defaults) =>
  switch (value) {
  | Int(value) => value
  | _ => defaults
  };

let unwrapBool = (value, defaults) =>
  switch (value) {
  | Bool(value) => value
  | _ => defaults
  };

let unwrapString = (value, defaults) =>
  switch (value) {
  | String(value) => value
  | _ => defaults
  };

let unwrapFloat = (value, defaults) =>
  switch (value) {
  | Float(value) => value
  | _ => defaults
  };
