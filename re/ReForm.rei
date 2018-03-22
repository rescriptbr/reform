module Helpers = ReForm_Helpers;

module Validation = ReForm_Validation;

module Value = ReForm_Value;

module type Config = {
  type state;
  type fields;
  let lens: list((fields, state => Value.t, (state, Value.t) => state));
};

module Create:
  (Config: Config) =>
  {
    type value = Value.t;
    type values = Config.state;
    type schema = list((Config.fields, Validation.validation(values)));
    type onSubmit = {
      values,
      setSubmitting: bool => unit,
      setError: option(string) => unit,
      resetFormState: unit => unit,
    };
    type state = {
      values,
      isSubmitting: bool,
      errors: list((Config.fields, option(string))),
      error: option(string),
    };
    type reform = {
      form: state,
      handleChange: (Config.fields, value) => unit,
      handleGlobalValidation: option(string) => unit,
      handleSubmit: unit => unit,
      getErrorForField: Config.fields => option(string),
    };
  };
