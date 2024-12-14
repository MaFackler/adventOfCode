type hand =
    | Rock
    | Paper
    | Scissors


let hand_to_int h =
    match h with
    | Rock -> 1
    | Paper -> 2
    | Scissors -> 3

let opponent_value c =
    match c with
    | 'A' -> Rock
    | 'B' -> Paper
    | 'C' -> Scissors
    | _ -> failwith (Printf.sprintf "Error: opponent %c" c)

let player_value c =
    match c with
    | 'X' -> Rock
    | 'Y' -> Paper
    | 'Z' -> Scissors
    | _ -> failwith (Printf.sprintf "Error: player %c" c)


let combat o p =
    if o = p then 3
    else if o = Rock && p = Scissors then 0
    else if o = Paper && p = Rock then 0
    else if o = Scissors && p = Paper then 0
    else if p = Rock && o = Scissors then 6
    else if p = Paper && o = Rock then 6
    else if p = Scissors && o = Paper then 6
    else failwith "Not possible"


let extract_line line =
    let first = String.get line 0 in
    let second = String.get line 2 in
    let o = opponent_value first in
    let p = player_value second in
    (combat o p) + hand_to_int(p)


let solve filename = 
    let contents = In_channel.with_open_bin filename In_channel.input_all in
    let lines = String.split_on_char '\n' contents in
    let lines = List.filter (fun x -> x <> "") lines in
    let values = List.map extract_line lines in
    let _ = List.map string_of_int values in
    let res = List.fold_left (+) 0 values in
    print_endline (string_of_int res)

let () =
    solve "data/day02.txt"
