module StateLenses = [%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
open BsReform;
module PostAddForm = ReForm.Make(StateLenses);

module PostAddMutationConfig = [%graphql
  {|
  mutation PostAddMutation ($description: String!, $title: String!) {
    createPost(description: $description, title: $title) {
      id
      title
      description
    }
  }
|}
];

module PostAddMutation =
  ReasonApolloHooks.Mutation.Make(PostAddMutationConfig);

[@react.component]
let make = () => {
  let mutate =
    PostAddMutation.use(
      ~options=PostAddMutation.options(~client=Apollo.client, ()),
      (),
    );
  let (result: option(PostAddMutation.result), setResult) =
    React.useState(() => None);

  let {state, submit, getFieldState, handleChange}: PostAddForm.api =
    PostAddForm.use(
      ~schema={
        PostAddForm.Validation.Schema([|
          Custom(
            Title,
            values =>
              Js.String.length(values.title) > 20
                ? Error("Keep it short!") : Valid,
          ),
          StringNonEmpty(Description),
          Custom(
            AcceptTerms,
            values =>
              values.acceptTerms == false
                ? Error("You must accept all the terms") : Valid,
          ),
        |]);
      },
      ~onSubmit=
        ({state}) => {
          mutate(
            PostAddMutation.options(
              ~variables=
                PostAddMutationConfig.make(
                  ~title=state.values.title,
                  ~description=state.values.description,
                  (),
                )##variables,
              (),
            ),
          )
          |> Js.Promise.then_(result =>
               setResult(_ => Some(result)) |> Js.Promise.resolve
             )
          |> ignore;

          None;
        },
      ~initialState={title: "", description: "", acceptTerms: false},
      (),
    );

  switch (result) {
  | Some(Error(_error)) =>
    <p> {ReasonReact.string("Something went wrong...")} </p>
  | Some(NoData) => <p> {ReasonReact.string("Something went wrong...")} </p>
  | Some(Data(data)) =>
    <div>
      <h2>
        {data##createPost
         |> Belt.Option.map(_, post =>
              "Post#" ++ post##id ++ " " ++ post##title
            )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </h2>
      <p>
        {data##createPost
         |> Belt.Option.map(_, post => post##description)
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </div>
  | None =>
    <form
      onSubmit={event => {
        ReactEvent.Synthetic.preventDefault(event);
        submit();
      }}>
      <label>
        <span> {"Title:" |> ReasonReact.string} </span>
        <input
          value={state.values.title}
          onChange={Helpers.handleChange(handleChange(Title))}
        />
        <p>
          {getFieldState(Field(Title))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> ReasonReact.string}
        </p>
      </label>
      <label>
        <span> {"Description:" |> ReasonReact.string} </span>
        <textarea
          value={state.values.description}
          rows=4
          onChange={Helpers.handleChange(handleChange(Description))}
        />
        <p>
          {getFieldState(Field(Description))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> ReasonReact.string}
        </p>
      </label>
      <label>
        <p>
          <span> {"Accept terms? " |> ReasonReact.string} </span>
          <input
            type_="checkbox"
            value={string_of_bool(state.values.acceptTerms)}
            onChange={event =>
              ReactEvent.Form.target(event)##checked
              |> handleChange(AcceptTerms)
            }
          />
        </p>
        <p>
          {getFieldState(Field(AcceptTerms))
           |> (
             fun
             | Error(error) => Some(error)
             | _ => None
           )
           |> Belt.Option.getWithDefault(_, "")
           |> ReasonReact.string}
        </p>
      </label>
      {state.formState == Submitting
         ? <p> {React.string("Saving...")} </p>
         : <button type_="submit"> {"Submit" |> ReasonReact.string} </button>}
    </form>
  };
};
