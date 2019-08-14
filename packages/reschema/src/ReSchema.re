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

  module RegExps = {
    let email = [%bs.re {|/.*@.*\..+/|}];
  };

  let validateField =
      (~validator, ~values, ~i18n: ReSchemaI18n.t): (field, fieldState) =>
    switch (validator) {
    | Validation.IntMin(field, min) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min ? Valid : Error(i18n.intMin(~value, ~min)),
      );
    | Validation.IntMax(field, max) =>
      let value = Lenses.get(values, field);

      (
        Field(field),
        value <= max ? Valid : Error(i18n.intMax(~value, ~max)),
      );
    | Validation.FloatMin(field, min) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min ? Valid : Error(i18n.floatMin(~value, ~min)),
      );
    | Validation.FloatMax(field, max) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid : Error(i18n.floatMax(~value, ~max)),
      );
    | Validation.Email(field) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(RegExps.email, value)
          ? Valid : Error(i18n.email(~value)),
      );
    | Validation.NoValidation(field) => (Field(field), Valid)
    | Validation.StringNonEmpty(field) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value === "" ? Error(i18n.stringNonEmpty(~value)) : Valid,
      );
    | Validation.StringRegExp(field, regexp) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(Js.Re.fromString(regexp), value)
          ? Valid : Error(i18n.stringRegExp(~value, ~pattern=regexp)),
      );
    | Validation.StringMin(field, min) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) >= min
          ? Valid : Error(i18n.stringMin(~value, ~min)),
      );
    | Validation.StringMax(field, max) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) <= max
          ? Valid : Error(i18n.stringMax(~value, ~max)),
      );
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

  let validateOne = (~field, ~values, ~i18n, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    getFieldValidator(~validators, ~fieldName=field)
    ->Belt.Option.map(validator => validateField(~validator, ~values, ~i18n));
  };

  let validate =
      (
        ~i18n=ReSchemaI18n.default,
        values: Lenses.state,
        schema: Validation.schema,
      ) => {
    let Validation.Schema(validators) = schema;

    let validationList =
      validators->Belt.Array.map(validator =>
        validateField(~validator, ~values, ~i18n)
      );

    let errors =
      validationList
      ->Belt.Array.keep(((_field, fieldState)) => fieldState !== Valid)
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
