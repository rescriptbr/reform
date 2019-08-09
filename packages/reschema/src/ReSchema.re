module type Lenses = {
  type field('a);
  type state;
  let set: (state, field('a), 'a) => state;
  let get: (state, field('a)) => 'a;
};

type fieldState =
  | Valid
  | Error(string);

type recordValidationState('a) =
  | Valid
  | Errors(array(('a, string)));

module Make = (Lenses: Lenses) => {
  type field =
    | Field(Lenses.field('a)): field;

  module Validation = {
    type t =
      | Email(Lenses.field(string)): t
      | NoValidation(Lenses.field('a)): t
      | StringNonEmpty(Lenses.field(string)): t
      | StringRegExp(Lenses.field(string), string): t
      | StringMin(Lenses.field(string), int): t
      | StringMax(Lenses.field(string), int): t
      | IntMin(Lenses.field(int), int): t
      | IntMax(Lenses.field(int), int): t
      | FloatMin(Lenses.field(float), float): t
      | FloatMax(Lenses.field(float), float): t
      | Custom(Lenses.field('a), Lenses.state => fieldState): t;
    type schema =
      | Schema(array(t)): schema;
  };

  let validateField = (~validator, ~values): (field, fieldState) =>
    switch (validator) {
    | Validation.IntMin(field, min) => (
        Field(field),
        Lenses.get(values, field) >= min
          ? Valid
          : Error(
              "This value must be greater than or equal to "
              ++ string_of_int(min),
            ),
      )
    | Validation.IntMax(field, max) => (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid
          : Error(
              "This value must be less than or equal to "
              ++ string_of_int(max),
            ),
      )
    | Validation.FloatMin(field, min) => (
        Field(field),
        Lenses.get(values, field) >= min
          ? Valid
          : Error(
              "This value must be greater than or equal to "
              ++ Js.Float.toString(min),
            ),
      )
    | Validation.FloatMax(field, max) => (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid
          : Error(
              "This value must be less than or equal to "
              ++ Js.Float.toString(max),
            ),
      )
    | Validation.Email(field) => (
        Field(field),
        Js.Re.test_(
          [%bs.re
            "/^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/"
          ],
          Lenses.get(values, field),
        )
          ? Valid : Error("Invalid email"),
      )
    | Validation.NoValidation(field) => (Field(field), Valid)
    | Validation.StringNonEmpty(field) => (
        Field(field),
        Lenses.get(values, field) === ""
          ? Error("String must not be empty") : Valid,
      )
    | Validation.StringRegExp(field, regexp) => (
        Field(field),
        Js.Re.test_(Js.Re.fromString(regexp), Lenses.get(values, field))
          ? Valid
          : Error("This value must match the following: /" ++ regexp ++ "/"),
      )
    | Validation.StringMin(field, min) => (
        Field(field),
        Js.String.length(Lenses.get(values, field)) >= min
          ? Valid
          : Error(
              "This value must be at least "
              ++ string_of_int(min)
              ++ " characters",
            ),
      )
    | Validation.StringMax(field, max) => (
        Field(field),
        Js.String.length(Lenses.get(values, field)) <= max
          ? Valid
          : Error(
              "This value must be at most "
              ++ string_of_int(max)
              ++ " characters",
            ),
      )
    | Validation.Custom(field, predicate) => (
        Field(field),
        predicate(values),
      )
    };

  let getFieldValidator = (~validators, ~fieldName) =>
    validators
    ->Belt.Array.keep(validator =>
        switch (validator) {
        | Validation.IntMin(field, _) => Field(field) == fieldName
        | Validation.IntMax(field, _) => Field(field) == fieldName
        | Validation.FloatMin(field, _) => Field(field) == fieldName
        | Validation.FloatMax(field, _) => Field(field) == fieldName
        | Validation.Email(field) => Field(field) == fieldName
        | Validation.NoValidation(field) => Field(field) == fieldName
        | Validation.StringNonEmpty(field) => Field(field) == fieldName
        | Validation.StringRegExp(field, _) => Field(field) == fieldName
        | Validation.StringMin(field, _) => Field(field) == fieldName
        | Validation.StringMax(field, _) => Field(field) == fieldName
        | Validation.Custom(field, _) => Field(field) == fieldName
        }
      )
    ->Belt.Array.get(0);

  let validateOne = (~field, ~values, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    getFieldValidator(~validators, ~fieldName=field)
    ->Belt.Option.map(validator => validateField(~validator, ~values));
  };

  let validate = (values: Lenses.state, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    let validationList =
      validators->Belt.Array.map(validator =>
        validateField(~validator, ~values)
      );

    let errors =
      validationList
      ->Belt.Array.keep(((field, fieldState)) => fieldState !== Valid)
      ->Belt.Array.map(((field, fieldState)) =>
          switch (fieldState) {
          | Valid => (field, None)
          | Error(message) => (field, Some(message))
          }
        )
      ->Belt.Array.map(((field, message)) =>
          (field, message->Belt.Option.getWithDefault(""))
        );

    Belt.Array.length(errors) > 0 ? Errors(errors) : Valid;
  };
};
