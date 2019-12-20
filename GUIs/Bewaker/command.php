<?php
error_reporting(E_ALL);
set_time_limit(0);
header('Content-Type: application/json');

define('PORT', 8883);
define('ADDRESS', '192.168.4.1');
define('SOCKSIZE', 256);

function connect() {
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if (!$socket) {
        exit;
    }
    $connection = socket_connect($socket, ADDRESS, PORT);
    if (!$connection) {
        exit;
    }

    return $socket;
}

$s = connect();

while($out = socket_read($s, SOCKSIZE)) {
    $msg = '';
    switch($out) {
    case "ID?\r":
        $msg = $_REQUEST['id'];
        break;
    case "OK\r":
        $msg = $_REQUEST['msg'];
        break;
    default:
        socket_close($s);
        break 2;
    }

    socket_write($s, $msg, strlen($msg));
}

echo json_encode(['success' => true, 'lamp' => $out]);
