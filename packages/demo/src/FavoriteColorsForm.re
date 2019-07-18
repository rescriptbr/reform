module FavoriteColorsLenses = [%lenses
  type t = {
    name: string,
    hex: string,
  }
];

module StateLenses = [%lenses
  type state = {
    name: string,
    favoriteColors: array(FavoriteColorsLenses.t),
    numberOfFavoriteColors: int,
  }
];

module Form = ReFormNext.Make(StateLenses);

[@react.component]
let make = () => {
  let {
    state,
    submit,
    getFieldState,
    handleChange,
    arrayPush,
    arrayUpdateByIndex,
    arrayRemoveByIndex,
  }: Form.api =
    Form.use(
      ~schema={
        Form.Validation.Schema([|
          Custom(
            Name,
            values =>
              Js.String.length(values.name) < 5
                ? Error("Invalid name") : Valid,
          ),
          Custom(
            FavoriteColors,
            ({favoriteColors}) => {
              let length = Array.length(favoriteColors);

              length < 0
              || Belt.Array.some(favoriteColors, favColor =>
                   Js.String.length(favColor.hex) == 0
                 )
                ? Error("Invalid colors") : Valid;
            },
          ),
          IntMin(NumberOfFavoriteColors, 1),
        |]);
      },
      ~onSubmit=({state}) => None,
      ~initialState={
        name: "",
        favoriteColors: [||],
        numberOfFavoriteColors: 0,
      },
      (),
    );

  <form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      submit();
    }}>
    <label>
      <span> {"Name:" |> ReasonReact.string} </span>
      <input
        value={state.values.name}
        onChange={ReForm.Helpers.handleDomFormChange(handleChange(Name))}
      />
      <p>
        {getFieldState(Field(Name))
         |> (
           fun
           | Error(error) => Some(error)
           | _ => None
         )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </label>
    <button onClick={_ => arrayPush(FavoriteColors, {name: "", hex: ""})}>
      {React.string("Add color")}
    </button>
    {state.values.favoriteColors
     ->Belt.Array.mapWithIndex((index, favColor) =>
         <>
           <hr />
           <label>
             <span> {"Name:" |> ReasonReact.string} </span>
             <input
               value={favColor.name}
               onChange={ReForm.Helpers.handleDomFormChange(name =>
                 arrayUpdateByIndex(
                   ~field=FavoriteColors,
                   ~index,
                   {...favColor, name},
                 )
               )}
             />
           </label>
           <label>
             <span> {"Hex:" |> ReasonReact.string} </span>
             <input
               value={favColor.hex}
               onChange={ReForm.Helpers.handleDomFormChange(hex =>
                 arrayUpdateByIndex(
                   ~field=FavoriteColors,
                   ~index,
                   {...favColor, hex},
                 )
               )}
             />
           </label>
           <button onClick={_ => arrayRemoveByIndex(FavoriteColors, index)}>
             {React.string("Remove")}
           </button>
           <hr />
         </>
       )
     ->React.array}
    <label>
      <span> {"Number of favorite colors:" |> ReasonReact.string} </span>
      <input
        value={string_of_int(state.values.numberOfFavoriteColors)}
        type_="number"
        onChange={ReForm.Helpers.handleDomFormChange(
          handleChange(NumberOfFavoriteColors),
        )}
      />
      <p>
        {getFieldState(Field(NumberOfFavoriteColors))
         |> (
           fun
           | Error(error) => Some(error)
           | _ => None
         )
         |> Belt.Option.getWithDefault(_, "")
         |> ReasonReact.string}
      </p>
    </label>
  </form>;
};
