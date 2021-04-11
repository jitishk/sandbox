define f1
set $node = (pttype *)$arg0
 if $node != 0x0
  set $intf = (nfm_intf_t *)$node->user_data
  echo node: $arg1 
  if $intf != 0x0
   print "intf ", $intf, $intf->ifindex
  end
  echo \n
  eval "set $count = $arg1 + 1"
  f1 $node->left $count
  f1 $node->right $count
 end
end

