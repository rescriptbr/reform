module PostAddFormParams = {
  type state = {
    description: string,
    title: string,
  };
  type fields = [ | `title | `description];
  /* (fieldName, getter, setter) */
  let lens = [
    (`title, s => s.title, (s, title) => {...s, title}),
    (
      `description,
      s => s.description,
      (s, description) => {...s, description},
    ),
  ];
};

module PostAddForm = ReForm.Create(PostAddFormParams);

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
  let mutate = PostAddMutation.use();
  let (result: option(PostAddMutation.result), setResult) =
    React.useState(() => None);

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
    <PostAddForm
      schema=[
        (
          `title,
          Custom(
            values =>
              Js.String.length(values.title) > 20
                ? Some("Keep it short!") : None,
          ),
        ),
        (`description, Required),
      ]
      onSubmit={({values}) =>
        mutate(
          ~variables=
            PostAddMutationConfig.make(
              ~title=values.title,
              ~description=values.description,
              (),
            )##variables,
          (),
        )
        |> Js.Promise.then_(result =>
             setResult(_ => Some(result)) |> Js.Promise.resolve
           )
        |> ignore
      }
      initialState={title: "", description: ""}>
      ...{({form, handleChange, handleSubmit, getErrorForField}) =>
        <form
          onSubmit={event => {
            ReactEvent.Synthetic.preventDefault(event);
            handleSubmit();
          }}>
          <label>
            <span> {"Title:" |> ReasonReact.string} </span>
            <input
              value={form.values.title}
              onChange={ReForm.Helpers.handleDomFormChange(
                handleChange(`title),
              )}
            />
            <p>
              {getErrorForField(`title)
               |> Belt.Option.getWithDefault(_, "")
               |> ReasonReact.string}
            </p>
          </label>
          <label>
            <span> {"Description:" |> ReasonReact.string} </span>
            <textarea
              value={form.values.description}
              rows=4
              onChange={ReForm.Helpers.handleDomFormChange(
                handleChange(`description),
              )}
            />
            <p>
              {getErrorForField(`description)
               |> Belt.Option.getWithDefault(_, "")
               |> ReasonReact.string}
            </p>
          </label>
          <button type_="submit"> {"Submit" |> ReasonReact.string} </button>
        </form>
      }
    </PostAddForm>
  };
};
