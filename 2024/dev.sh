session='advent'

tmux new-session -d -s $session
tmux split-pane -h -p 20 -t $session
tmux split-pane -v  -t $session
tmux select-pane -t 1
tmux send-keys 'make entr' C-m
tmux select-pane -t 0
tmux send-keys 'vim .' C-m
tmux attach -t $session

