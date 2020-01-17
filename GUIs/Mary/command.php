<?php
error_reporting(E_ALL);
set_time_limit(0);
header('Content-Type: application/json');

define('PORT', 8883);
define('ADDRESS', '192.168.4.1');
define('SOCKSIZE', 1024);


function connect()
{
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if (!$socket) {
        exit;
    }

    socket_set_option($socket, getprotobyname('tcp'), SO_RCVTIMEO, ['sec' => 0, 'usec' => 500]);
    $connection = socket_connect($socket, ADDRESS, PORT);

    if (!$connection) {
        exit;
    }

    return $socket;

}//end connect()


$s = connect();

while ($out = socket_read($s, SOCKSIZE)) {
    $msg = '';
    switch ($out) {
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

if ($out !== 'null') {
    echo $out;
} else {
    echo json_encode(['success' => false]);
}
