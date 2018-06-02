<?php
error_reporting(0);
$service_port = 8000;
$address = gethostbyname('localhost');
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    echo "Hub ของบ้านเกิดปัญหา กรุณารีสตาร์ทอุปกรณ์ด้วยครับ";
} else {
    $result = socket_connect($socket, $address, $service_port);
    if ($result === false) {
        echo "Hub ของบ้านเกิดปัญหา กรุณารีสตาร์ทอุปกรณ์ด้วยครับ";
    } else {
        $order = $_POST['value'];
        $out = '';
        ocket_write($socket, $order, strlen($order));
        /*
        while ($out = socket_read($socket, 2048)) {
            echo $out;
        }*/
        socket_close($socket);
        echo('สั่งงานอุปกรณ์สำเร็จ!');
    }
}
?>