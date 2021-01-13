open Higher;

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
  | Errors(array(('a, string)));

module Validation = {
  type t('meta, 'state) =
    | Email({
        field: app(string, 'field),
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | NoValidation({
        field: app('a, 'field),
        meta: 'b,
      })
      : t('meta, 'state)
    | StringNonEmpty({
        field: app(string, 'field),
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | StringRegExp({
        field: app(string, 'field),
        matches: string,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | StringMin({
        field: app(string, 'field),
        min: int,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | StringMax({
        field: app(string, 'field),
        max: int,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | IntMin({
        field: app(int, 'field),
        min: int,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | IntMax({
        field: app(int, 'field),
        max: int,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | FloatMin({
        field: app(float, 'field),
        min: float,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | FloatMax({
        field: app(float, 'field),
        max: float,
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | Custom({
        field: app('a, 'field),
        meta: 'meta,
        predicate: 'state => fieldState,
      })
      : t('meta, 'state)
    | True({
        field: app(bool, 'field),
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'state)
    | False({
        field: app(bool, 'field),
        error: option(string),
        meta: 'meta,
      })
      : t('meta, 'field);
  type schema('meta) =
    | Schema(array(t('meta, 'state))): schema('meta);
  let (+) = (a, b) => a->Belt.Array.concat(b);
  let (<?) = (arr, maybeArr) => {
    switch (maybeArr) {
    | Some(someArr) => arr + [|someArr|]
    | None => arr
    };
  };

  let custom = (predicate, ~meta=?, field) => {
    [|Custom({field, meta, predicate})|];
  };
  let true_ = (~error=?, ~meta=?, field) => [|True({field, meta, error})|];

  let false_ = (~error=?, ~meta=?, field) => [|
    False({field, meta, error}),
  |];

  let email = (~error=?, ~meta=?, field) => [|Email({field, meta, error})|];

  let nonEmpty = (~error=?, ~meta=?, field) => [|
    StringNonEmpty({field, meta, error}),
  |];

  let string = (~min=?, ~minError=?, ~max=?, ~maxError=?, ~meta=?, field) => {
    Belt.Option.(
      [||]
      <? min->map(min => StringMin({field, meta, min, error: minError}))
      <? max->map(max => StringMax({field, meta, max, error: maxError}))
    );
  };

  let regExp = (~error=?, ~matches, ~meta=?, field) => {
    [|StringRegExp({field, meta, matches, error})|];
  };

  let float = (~min=?, ~minError=?, ~max=?, ~maxError=?, ~meta=?, field) => {
    Belt.Option.(
      [||]
      <? min->map(min => FloatMin({field, meta, min, error: minError}))
      <? max->map(max => FloatMax({field, meta, max, error: maxError}))
    );
  };

  let int = (~min=?, ~minError=?, ~max=?, ~maxError=?, ~meta=?, field) => {
    Belt.Option.(
      [||]
      <? min->map(min => IntMin({field, meta, min, error: minError}))
      <? max->map(max => IntMax({field, meta, max, error: maxError}))
    );
  };
};

module Make = (Lenses: Lenses) => {
  type field =
    | Field(Lenses.field('a)): field;

  module RegExps = {
    [@bs.module "./ReSchemaRegExp"] external email: Js.Re.t = "email";
  };

  let validateField =
      (~validator, ~values, ~i18n: ReSchemaI18n.t): (field, fieldState) =>
    switch (validator) {
    | Validation.True({field, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value
          ? Valid : Error(error->Belt.Option.getWithDefault(i18n.true_())),
      );
    | Validation.False({field, error}) =>
      let value = Lenses.get(values, field);
      (
        Field(field),
        value == false
          ? Valid : Error(error->Belt.Option.getWithDefault(i18n.false_())),
      );
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
      | Validation.False({field}) => Field(field) == fieldName
      | Validation.True({field}) => Field(field) == fieldName
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

  let validateOne =
      (~field, ~values, ~i18n, schema: Validation.schema('meta)) => {
    let Validation.Schema(validators) = schema;

    getFieldValidator(~validators, ~fieldName=field)
    ->Belt.Option.map(validator => validateField(~validator, ~values, ~i18n));
  };

  let validateFields =
      (~fields, ~values, ~i18n, schema: Validation.schema('meta)) => {
    let Validation.Schema(validators) = schema;

    Belt.Array.map(fields, field =>
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
        schema: Validation.schema('meta),
      ) => {
    let Validation.Schema(validators) = schema;

    let validationList =
      validators->Belt.Array.map(validator =>
        validateField(~validator, ~values, ~i18n)
      );

    let errors =
      validationList->Belt.Array.keepMap(((field, fieldState)) =>
        switch (fieldState) {
        | Error(string) => Some((field, string))
        | _ => None
        }
      );

    Belt.Array.length(errors) > 0 ? Errors(errors) : Valid;
  };
};
