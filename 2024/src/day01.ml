
let file = "data/day01.txt"

let ignore_empty lst =
    List.filter (fun x -> x <> "") lst


let get_by_index index line = 
    let splitted = String.split_on_char ' ' line in
    let splitted = ignore_empty splitted in
    int_of_string(List.nth splitted index)

let part1 filename =
    let contents = In_channel.with_open_bin filename In_channel.input_all in
    let lines = String.split_on_char '\n' contents in
    let lines = ignore_empty lines in
    let left = List.map (get_by_index 0) lines in
    let right = List.map (get_by_index 1) lines in
    let left = List.sort compare left in
    let right = List.sort compare right in
    let differences = List.mapi (fun index ele -> abs(ele - (List.nth left index))) right in
    List.fold_left (+) 0 differences

let () =
    let res = part1 file in
    print_endline (string_of_int res)
