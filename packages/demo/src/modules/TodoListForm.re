open BsReform;

module TodoLenses = [%lenses
  type todo = {
    content: string,
    completed: bool,
  }
];

module StateLenses = [%lenses type state = {todos: array(TodoLenses.todo)}];

module TodoForm = ReForm.Make(StateLenses);

[@react.component]
let make = () => {
  let {state, submit, arrayPush, arrayUpdateByIndex, arrayRemoveByIndex}: TodoForm.api =
    TodoForm.use(
      ~schema={
        TodoForm.Validation.(
          Schema(
            custom(
              ({todos}) => {
                let length = Array.length(todos);

                length < 0
                || Belt.Array.some(todos, item =>
                     Js.String.length(item.content) == 0
                   )
                  ? Error("Invalid todo") : Valid;
              },
              Todos,
            ),
          )
        );
      },
      ~onSubmit=
        ({state}) => {
          Js.log2("State", state);
          None;
        },
      ~initialState={todos: [||]},
      (),
    );

  <div>
    <form
      onSubmit={event => {
        ReactEvent.Synthetic.preventDefault(event);
        submit();
      }}>
      <button
        onClick={_ => arrayPush(Todos, {content: "", completed: false})}>
        {"Add Task" |> React.string}
      </button>
      {state.values.todos
       ->Belt.Array.mapWithIndex((index, todo) =>
           <>
             <hr />
             <label>
               <span> {" Task description " |> React.string} </span>
               <input
                 value={todo.content}
                 onChange={BsReform.Helpers.handleChange(content =>
                   arrayUpdateByIndex(
                     ~field=Todos,
                     ~index,
                     {...todo, content},
                   )
                 )}
               />
             </label>
             <span>
               {(todo.completed ? "Done" : "Not done") |> React.string}
             </span>
             <button
               onClick={_ =>
                 arrayUpdateByIndex(
                   ~field=Todos,
                   ~index,
                   {...todo, completed: !todo.completed},
                 )
               }>
               {"Toggle" |> React.string}
             </button>
             <button onClick={_ => arrayRemoveByIndex(Todos, index)}>
               {"Remove" |> React.string}
             </button>
           </>
         )
       ->React.array}
    </form>
  </div>;
};
