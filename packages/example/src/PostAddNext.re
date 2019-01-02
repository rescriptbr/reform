[%lenses
  type state = {
    description: string,
    title: string,
    acceptTerms: bool,
  }
];
module PostAddForm = ReFormNext.Make(StateLenses);

module PostAddMutation = [%graphql
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

module MutationContainer = ReasonApollo.CreateMutation(PostAddMutation);

let component = ReasonReact.statelessComponent("PostAdd");

let make = _children => {
  ...component,
  render: _self =>
    <MutationContainer>
      ...{
           (mutate, {result}) =>
             switch (result) {
             | Loading => <p> {ReasonReact.string("Loading...")} </p>
             | Error(_error) =>
               <p> {ReasonReact.string("Something went wrong...")} </p>
             | Data(data) =>
               <div>
                 <h2>
                   {
                     data##createPost
                     |> Belt.Option.map(_, post =>
                          "Post#" ++ post##id ++ " " ++ post##title
                        )
                     |> Belt.Option.getWithDefault(_, "")
                     |> ReasonReact.string
                   }
                 </h2>
                 <p>
                   {
                     data##createPost
                     |> Belt.Option.map(_, post => post##description)
                     |> Belt.Option.getWithDefault(_, "")
                     |> ReasonReact.string
                   }
                 </p>
               </div>
             | NotCalled =>
               <PostAddForm
                 schema={
                   PostAddForm.Validation.Schema([|
                     Custom(
                       Title,
                       (
                         values =>
                           Js.String.length(values.title) > 20 ?
                             Error("Keep it short!") : Valid
                       ),
                     ),
                     Custom(
                       Description,
                       (
                         values =>
                           values.description == "" ?
                             Error("Can't be empty") : Valid
                       ),
                     ),
                     Custom(
                       AcceptTerms,
                       (
                         values =>
                           values.acceptTerms == false ?
                             Error("You must accept all the terms") : Valid
                       ),
                     ),
                   |])
                 }
                 onSubmit=(
                   ({state}) =>
                     mutate(
                       ~variables=
                         PostAddMutation.make(
                           ~title=state.values.title,
                           ~description=state.values.description,
                           (),
                         )##variables,
                       (),
                     )
                     |> ignore
                 )
                 initialState={title: "", description: "", acceptTerms: false}>
                 ...(
                      ({state, send, getFieldState}) =>
                        <form
                          onSubmit={
                            event => {
                              ReactEvent.Synthetic.preventDefault(event);
                              send(TrySubmit);
                            }
                          }>
                          <label>
                            <span> {"Title:" |> ReasonReact.string} </span>
                            <input
                              value={state.values.title}
                              onChange={
                                ReForm.Helpers.handleDomFormChange(value =>
                                  send(FieldChangeValue(Title, value))
                                )
                              }
                            />
                            <p>
                              {
                                getFieldState(Field(Title))
                                |> (
                                  fun
                                  | Error(error) => Some(error)
                                  | _ => None
                                )
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.string
                              }
                            </p>
                          </label>
                          <label>
                            <span>
                              {"Description:" |> ReasonReact.string}
                            </span>
                            <textarea
                              value={state.values.description}
                              rows=4
                              onChange={
                                ReForm.Helpers.handleDomFormChange(value =>
                                  send(FieldChangeValue(Description, value))
                                )
                              }
                            />
                            <p>
                              {
                                getFieldState(Field(Description))
                                |> (
                                  fun
                                  | Error(error) => Some(error)
                                  | _ => None
                                )
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.string
                              }
                            </p>
                          </label>
                          <label>
                            <p>
                              <span>
                                {"Accept terms? " |> ReasonReact.string}
                              </span>
                              <input
                                type_="checkbox"
                                value={
                                  string_of_bool(state.values.acceptTerms)
                                }
                                onChange={
                                  event => {
                                    let checked =
                                      ReactEvent.Form.target(event)##checked;
                                    send(
                                      FieldChangeValue(AcceptTerms, checked),
                                    );
                                  }
                                }
                              />
                            </p>
                            <p>
                              {
                                getFieldState(Field(AcceptTerms))
                                |> (
                                  fun
                                  | Error(error) => Some(error)
                                  | _ => None
                                )
                                |> Belt.Option.getWithDefault(_, "")
                                |> ReasonReact.string
                              }
                            </p>
                          </label>
                          <button type_="submit">
                            {"Submit" |> ReasonReact.string}
                          </button>
                        </form>
                    )
               </PostAddForm>
             }
         }
    </MutationContainer>,
};
