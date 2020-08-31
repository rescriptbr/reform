[@react.component]
let make = (~message) => {
  let url = ReasonReactRouter.useUrl();

  <div className="p-5">
    <div className="flex align-center justify-center">
      <h2 className="text-2xl"> {React.string(message)} </h2>
    </div>
    <ul className="flex mt-6">
      <li className="mr-6">
        <a className="text-blue-500 hover:text-blue-800" href="/new-post">
          "New post form demo "->React.string
        </a>
      </li>
      <li className="mr-6">
        <a className="text-blue-500 hover:text-blue-800" href="/fav-colors">
          "Form with array field demo"->React.string
        </a>
      </li>
      <li className="mr-6">
        <a className="text-blue-500 hover:text-blue-800" href="/todo">
          "Form with array todo demo"->React.string
        </a>
      </li>
      <li className="mr-6">
        <a className="text-blue-500 hover:text-blue-800" href="/reschema">
          "Vanilla ReSchema"->React.string
        </a>
      </li>
    </ul>
    <div style={ReactDOMRe.Style.make(~padding="100px", ())}>
      {switch (url.path) {
       | ["reschema"] => <VanillaReSchema />
       | ["new-post"] => <PostAddNext />
       | ["fav-colors"] => <FavoriteColorsForm />
       | ["todo"] => <TodoListForm />
       | _ =>
         <p className="App-intro">
           {ReasonReact.string("Say hello to ReForm")}
         </p>
       }}
    </div>
  </div>;
};
