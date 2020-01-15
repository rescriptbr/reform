module type Lenses = {
  type field('a);
  type state;
  let set: (state, field('a), 'a) => state;
  let get: (state, field('a)) => 'a;
};

type childFieldError = {
  error: string,
  index: int,
  name: string,
};

type fieldState =
  | Valid
  | NestedErrors(array(childFieldError))
  | Error(string);

type recordValidationState('a) =
  | Valid
  | Errors(array(('a, fieldState)));

module Make = (Lenses: Lenses) => {
  type field =
    | Field(Lenses.field('a)): field;

  module Validation = {
    type t =
      | Email(Lenses.field(string), option(string)): t
      | NoValidation(Lenses.field('a)): t
      | StringNonEmpty(Lenses.field(string), option(string)): t
      | StringRegExp(Lenses.field(string), string, option(string)): t
      | StringMin(Lenses.field(string), int, option(string)): t
      | StringMax(Lenses.field(string), int, option(string)): t
      | IntMin(Lenses.field(int), int, option(string)): t
      | IntMax(Lenses.field(int), int, option(string)): t
      | FloatMin(Lenses.field(float), float, option(string)): t
      | FloatMax(Lenses.field(float), float, option(string)): t
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
    | Validation.IntMin(field, min, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.intMin(~value, ~min),
              ),
            ),
      );
    | Validation.IntMax(field, max, customErrorMessage) =>
      let value = Lenses.get(values, field);

      (
        Field(field),
        value <= max
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.intMax(~value, ~max),
              ),
            ),
      );
    | Validation.FloatMin(field, min, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.floatMin(~value, ~min),
              ),
            ),
      );
    | Validation.FloatMax(field, max, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.floatMax(~value, ~max),
              ),
            ),
      );
    | Validation.Email(field, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(RegExps.email, value)
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.email(~value),
              ),
            ),
      );
    | Validation.NoValidation(field) => (Field(field), Valid)
    | Validation.StringNonEmpty(field, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value === ""
          ? Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.stringNonEmpty(~value),
              ),
            )
          : Valid,
      );
    | Validation.StringRegExp(field, regexp, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(Js.Re.fromString(regexp), value)
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.stringRegExp(~value, ~pattern=regexp),
              ),
            ),
      );
    | Validation.StringMin(field, min, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) >= min
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.stringMin(~value, ~min),
              ),
            ),
      );
    | Validation.StringMax(field, max, customErrorMessage) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) <= max
          ? Valid
          : Error(
              customErrorMessage->Belt.Option.getWithDefault(
                i18n.stringMax(~value, ~max),
              ),
            ),
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
        | Validation.IntMin(field, _, _) => Field(field) == fieldName
        | Validation.IntMax(field, _, _) => Field(field) == fieldName
        | Validation.FloatMin(field, _, _) => Field(field) == fieldName
        | Validation.FloatMax(field, _, _) => Field(field) == fieldName
        | Validation.Email(field, _) => Field(field) == fieldName
        | Validation.NoValidation(field) => Field(field) == fieldName
        | Validation.StringNonEmpty(field, _) => Field(field) == fieldName
        | Validation.StringRegExp(field, _, _) => Field(field) == fieldName
        | Validation.StringMin(field, _, _) => Field(field) == fieldName
        | Validation.StringMax(field, _, _) => Field(field) == fieldName
        | Validation.Custom(field, _) => Field(field) == fieldName
        }
      )
    ->Belt.Array.get(0);

  let validateOne = (~field, ~values, ~i18n, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    getFieldValidator(~validators, ~fieldName=field)
    ->Belt.Option.map(validator => validateField(~validator, ~values, ~i18n));
  };

  let validateFields = (~fields, ~values, ~i18n, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    Belt.Array.map(fields, field =>
      getFieldValidator(~validators, ~fieldName=field)
    )
    ->Belt.Array.map(validator =>
        Belt.Option.map(validator, validator =>
          validateField(~validator, ~values, ~i18n)
        )
      );
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
      ->Belt.Array.map(((field, fieldState)) => (field, fieldState));

    Belt.Array.length(errors) > 0 ? Errors(errors) : Valid;
  };
};
