#ifndef controlpage_H
#define controlpage_H

const char controlpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controller</title>
    <style>
        :root{
            --dark:#333;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        html {
            height: 100%;
        }

        body {
            height: 99%;
            background-image: url(/bg13_1.png);
            font-family: 'Press Start 2P', cursive;
            background-size: cover;
            background-repeat: no-repeat;
            
        }
        
        .topbar {
            display: flex;
            justify-content: space-between;
            align-items: center;
            background: linear-gradient(to right, var(--dark), blue, var(--dark));
            color: #fff;
        }
        
        .logo-container {
            width: 10%;
            height: 100%;
            display: flex;
            align-items: center;
        }
        
        .logo {
            max-width: 100%;
            max-height: 100%;
        }
        
        .title-container {
            text-align: center;
        }

        .navbar {
            text-align: center;
            background-color: #333;
            display: flex;
            justify-content: flex-end;
        }

        .navbar a {
            color: white;
            text-decoration: none;
        }

        .content-container {
            padding: 20px;
            height: 78%;
        }

        #display {
            color: black;
            text-align: center;
        }

        .controller {
            display: flex;
            justify-content: space-evenly;
            position: relative;
            top: 26%;
        }

        .left{
            display: flex;
            flex-direction: column;
            justify-content: space-evenly;
            align-items: center;
        }

        .right{
            display: flex;
            flex-direction: column;
            justify-content: space-evenly;
            align-items: center;
        }

        .joystick-container {
            width: 400px;
            height: 400px;
            display: flex;
            align-items: center;
            justify-content: center;
            flex-direction: column;
        }

        .joystick {
            width: 70%;
            height: 70%;
            background-color: #333;
            border-radius: 50%;
        }


        .handle {
            width: 35%;
            height: 35%;
            background-color: royalblue;
            border-radius: 50%;
            position: relative;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            cursor: pointer;
        }

        .circular-button {
            height: 100px;
            width: 100px;
            border-radius: 50%;
            background-color: black;
            color: white;
            text-align: center;
        }

        .circular-button:hover {
            cursor: pointer;
        }

        @media screen and (max-width: 768px)
        {

            div{
                border: 2px solid red;
            }
            /* body{
                background:white;
            } */
            .topbar{
                font-size: 0.4rem;
            }

            .navbar{
                font-size: x-small;
            }

            /* #display{
                color: black;
            */

            .joystick-container{
                height: 300px;
                width: 300px;
            }

            .left,.right{
                flex-direction: row;
            }
            .circular-button{
                height: 50px;
                width: 50px;
                /*background-color: black; */
            }

            .controller{
                flex-direction: column;
            }

            .joystick-container{
                align-self: center;
            }
        }
    </style>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap" rel="stylesheet">
</head>

<body>
    <div class="topbar">
        <div class="logo-container">
            <a href="https://svnit.ac.in"><img src="/svnit_logo.png" alt="Logo 1" class="logo"></a>
        </div>
        <div class="title-container">
            <h1>OMNI WEB DRIVE</h1>
        </div>
        <div class="logo-container">
            <a href="https://drishti-svnit.github.io/drishti/"><img src="/drishti_logo_white.png" alt="Logo 2" class="logo"></a>
        </div>
    </div>

    <div class="navbar">
        <a href="developers.html">Developers</a>
    </div>

    <div class="content-container">
        <div id="display">
            (X,Y)
        </div>

        <div class="controller">
            <div class="left">
                <div class="circular-button top-left" title="Anti-clockwise 360 movement of bot around it's own axis on 'Click!' "></div>
                <div class="circular-button bottom-left">ON</div>
            </div>
            <div class="joystick-container" title="Control the Omnibot with this joystick to move in any direction">
                <div class="joystick">
                    <div class="handle" id="handle"></div>
                </div>
            </div>
            <div class="right">
                <div class="circular-button top-right" title="Clockwise 360 movement of bot around it's own axis on 'Click!' "></div>
                <div class="circular-button bottom-right"></div>
            </div>
        </div>

    </div>


    <script>
        const joystick = document.querySelector('.joystick');
        const handle = document.querySelector('.handle');
        const websockets = new WebSocket("ws://"+location.hostname+":81/"); // Replace with your ESP32's IP address
        var button = document.getElementsByClassName("bottom-left")[0];
        var isActive = false;

        button.addEventListener("click", function () {
            isActive = !isActive;
            if(button.innerText=='ON')
            {
                button.innerText="OFF";
            }
            else
            button.innerText="ON";
        });

        let isJoystickPressed = false;
        let normX = 0;
        let normY = 0;

        joystick.addEventListener('mousedown', (e) => {
            isJoystickPressed = true;
            updateJoystick(e);
        });
        joystick.addEventListener('touchstart', (e) => {
            e.preventDefault();
            isJoystickPressed = true;
            updateJoystick(e.touches[0]);
        })


        document.addEventListener('mousemove', (e) => {
            if (isJoystickPressed) {
                updateJoystick(e);
            }
        });
        joystick.addEventListener('touchmove', (e) => {
            e.preventDefault();
            if (isJoystickPressed) {
                updateJoystick(e.touches[0]);
            }
        });

        document.addEventListener('mouseup', (e) => {
            isJoystickPressed = false;
            updateJoystick(e);
            handle.style.transform = 'translate(-50%, -50%)';
            console.log("Normalized X:", normX);
            console.log("Normalized Y:", normY);
            document.getElementById('display').innerText = '(' + normX + ',' + normY + ')';
        });
        joystick.addEventListener('touchend', (e) => {
            e.preventDefault();
            isJoystickPressed = false;
            updateJoystick(e);
            handle.style.transform = 'translate(-50%, -50%)';
            console.log("Normalized X:", normX);
            console.log("Normalized Y:", normY);
            document.getElementById('display').innerText = '(' + normX + ',' + normY + ')';
        });

        function updateJoystick(e) {
            const bounds = joystick.getBoundingClientRect();
            const centerX = bounds.left + bounds.width / 2;
            const centerY = bounds.top + bounds.height / 2;

            const deltaX = e.clientX - centerX;
            const deltaY = e.clientY - centerY;

            const distance = Math.min(joystick.offsetWidth / 2, Math.sqrt(deltaX ** 2 + deltaY ** 2));
            const angle = Math.atan2(deltaY, deltaX);

            const maxX = Math.cos(angle) * distance;
            const maxY = Math.sin(angle) * distance;

            handle.style.transform = `translate(calc(-50% + ${maxX}px), calc(-50% + ${maxY}px))`;

            // Normalize values between -1 and 1
            normX = maxX / (joystick.offsetWidth / 2);
            normY = -maxY / (joystick.offsetHeight / 2);

            normX=normX*128;
            normY=normY*128;

            if(!isJoystickPressed)
                {
                    normX=0;
                    normY=0;
                }

            // Use normX and normY for your application
            console.log("Normalized X:", normX);
            console.log("Normalized Y:", normY);
            document.getElementById('display').innerText = '(' + normX.toFixed(2) + ',' + normY.toFixed(2) + ')';
            document.getElementById('display').innerText = '(' + normX.toFixed(2) + ',' + normY.toFixed(2) + ')';

            const joystickData = {
                x:normX,
                y:-normY,
                state: isActive
              };
              websockets.send(JSON.stringify(joystickData));
        }


    </script>
</body>

</html>

)=====";

#endif
