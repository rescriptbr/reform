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
      | Email({
          field: Lenses.field(string),
          error: option(string),
        })
        : t
      | NoValidation({field: Lenses.field('a)}): t
      | StringNonEmpty({
          field: Lenses.field(string),
          error: option(string),
        })
        : t
      | StringRegExp({
          field: Lenses.field(string),
          matches: string,
          error: option(string),
        })
        : t
      | StringMin({
          field: Lenses.field(string),
          min: int,
          error: option(string),
        })
        : t
      | StringMax({
          field: Lenses.field(string),
          max: int,
          error: option(string),
        })
        : t
      | IntMin({
          field: Lenses.field(int),
          min: int,
          error: option(string),
        })
        : t
      | IntMax({
          field: Lenses.field(int),
          max: int,
          error: option(string),
        })
        : t
      | FloatMin({
          field: Lenses.field(float),
          min: float,
          error: option(string),
        })
        : t
      | FloatMax({
          field: Lenses.field(float),
          max: float,
          error: option(string),
        })
        : t
      | Custom({
          field: Lenses.field('a),
          predicate: Lenses.state => fieldState,
        })
        : t;
    type schema =
      | Schema(array(t)): schema;

    let (+) = (a, b) => a->Belt.Array.concat(b);
    let (<?) = (arr, maybeArr) => {
      switch (maybeArr) {
      | Some(someArr) => arr + [|someArr|]
      | None => arr
      };
    };

    let custom = (predicate, field) => {
      [|Custom({field, predicate})|];
    };

    let email = (~error=?, field) => [|Email({field, error})|];

    let nonEmpty = (~error=?, field) => [|StringNonEmpty({field, error})|];

    let string = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      Belt.Option.(
        [||]
        <? min->map(min => StringMin({field, min, error: minError}))
        <? max->map(max => StringMax({field, max, error: maxError}))
      );
    };

    let regExp = (~error=?, ~matches, field) => {
      [|StringRegExp({field, matches, error})|];
    };

    let float = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      Belt.Option.(
        [||]
        <? min->map(min => FloatMin({field, min, error: minError}))
        <? max->map(max => FloatMax({field, max, error: maxError}))
      );
    };

    let int = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      Belt.Option.(
        [||]
        <? min->map(min => IntMin({field, min, error: minError}))
        <? max->map(max => IntMax({field, max, error: maxError}))
      );
    };
  };

  module RegExps = {
    let email = [%bs.re {|/.*@.*\..+/|}];
  };

  let validateField =
      (~validator, ~values, ~i18n: ReSchemaI18n.t): (field, fieldState) =>
    switch (validator) {
    | Validation.IntMin({field, min, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(i18n.intMin(~value, ~min)),
            ),
      );
    | Validation.IntMax({field, max, error}) =>
      let value = Lenses.get(values, field);

      (
        Field(field),
        value <= max
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(i18n.intMax(~value, ~max)),
            ),
      );
    | Validation.FloatMin({field, min, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value >= min
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(i18n.floatMin(~value, ~min)),
            ),
      );
    | Validation.FloatMax({field, max, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(i18n.floatMax(~value, ~max)),
            ),
      );
    | Validation.Email({field, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(RegExps.email, value)
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.email(~value))),
      );
    | Validation.NoValidation({field}) => (Field(field), Valid)
    | Validation.StringNonEmpty({field, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value === ""
          ? Error(
              error->Belt.Option.getWithDefault(i18n.stringNonEmpty(~value)),
            )
          : Valid,
      );
    | Validation.StringRegExp({field, matches, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.Re.test_(Js.Re.fromString(matches), value)
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(
                i18n.stringRegExp(~value, ~pattern=matches),
              ),
            ),
      );
    | Validation.StringMin({field, min, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) >= min
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(
                i18n.stringMin(~value, ~min),
              ),
            ),
      );
    | Validation.StringMax({field, max, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        Js.String.length(value) <= max
          ? Valid
          : Error(
              error->Belt.Option.getWithDefault(
                i18n.stringMax(~value, ~max),
              ),
            ),
      );
    | Validation.Custom({field, predicate}) => (
        Field(field),
        predicate(values),
      )
    };

  let getFieldValidator = (~validators, ~fieldName) =>
    validators->Belt.Array.getBy(validator =>
      switch (validator) {
      | Validation.IntMin({field}) => Field(field) == fieldName
      | Validation.IntMax({field}) => Field(field) == fieldName
      | Validation.FloatMin({field}) => Field(field) == fieldName
      | Validation.FloatMax({field}) => Field(field) == fieldName
      | Validation.Email({field}) => Field(field) == fieldName
      | Validation.NoValidation({field}) => Field(field) == fieldName
      | Validation.StringNonEmpty({field}) => Field(field) == fieldName
      | Validation.StringRegExp({field}) => Field(field) == fieldName
      | Validation.StringMin({field}) => Field(field) == fieldName
      | Validation.StringMax({field}) => Field(field) == fieldName
      | Validation.Custom({field}) => Field(field) == fieldName
      }
    );

  let validateOne = (~field, ~values, ~i18n, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    getFieldValidator(~validators, ~fieldName=field)
    ->Belt.Option.map(validator => validateField(~validator, ~values, ~i18n));
  };

  let validateFields = (~fields, ~values, ~i18n, schema: Validation.schema) => {
    let Validation.Schema(validators) = schema;

    fields->Belt.Array.map(field =>
      getFieldValidator(~validators, ~fieldName=field)
      ->Belt.Option.map(validator =>
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
      validationList->Belt.Array.keep(((_field, fieldState)) =>
        fieldState !== Valid
      );

    Belt.Array.length(errors) > 0 ? Errors(errors) : Valid;
  };
};
