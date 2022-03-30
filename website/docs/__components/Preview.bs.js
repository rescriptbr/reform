

import * as React from "react";
import * as Ancestor from "@rescriptbr/ancestor/src/Ancestor.bs.js";

function $$default(props) {
  return React.createElement(Ancestor.Default.Box.make, {
              borderRadius: [Ancestor.Default.xs(1)],
              border: [Ancestor.Default.xs([
                      {
                        NAME: "px",
                        VAL: 1
                      },
                      "solid",
                      {
                        NAME: "hex",
                        VAL: "#cecece"
                      }
                    ])],
              bgColor: [Ancestor.Default.xs({
                      NAME: "hex",
                      VAL: "#fff"
                    })],
              p: [Ancestor.Default.xs(4)],
              children: props.children
            });
}

export {
  $$default ,
  $$default as default,
  
}
/* react Not a pure module */
