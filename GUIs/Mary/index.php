<!doctype html>
<html lang="nl">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1"/>
        <meta charset="UTF-8"/>
        <title>Mary - Control panel</title>
        <link href="semantic/dist/semantic.min.css" rel="stylesheet"/>
        <script src="jquery-3.4.1.min.js"></script>
        <script src="semantic/dist/semantic.min.js"></script>
        <script src="script.js"></script>
    </head>
    <body>
        <div class="ui top attached very padded segment">
            <div class="ui grid">
                <div class="eight wide column">
                    <h1 class="ui header"><i class="desktop icon"></i><div class="content">Mary - Control Panel<div class="sub header">Made by van Adrichem, Hooghuis, Meissen, Olsthoorn</div></div></h1>
                </div>
                <div class="eight wide column">
                    <button id="mary-nood-knop" class="ui red massive labeled icon button">
                        <i class="exclamation triangle icon" style="scroll-behavior: unset;"></i>
                        NOOD
                    </button>
                </div>
            </div>
        </div>

        <div class="ui attached very padded container segment">

            <div class="ui cards">

                <div class="ui red schemerlamp card">
                    <div class="image">
                        <img alt="" src="img/schemerlamp.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Schemerlamp</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided two column grid">
                                <div class="column lamp">
                                    <p><b>Lamp</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                                <div class="column disco">
                                    <p><b>Discostand</b></p>
                                    <div class="ui fitted slider checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ui orange stoel card">
                    <div class="image">
                        <img alt="" src="img/stoel.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Stoel</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided two column grid">
                                <div class="column trillen">
                                    <p><b>Trillen</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                                <div class="column lamp">
                                    <p><b>Lamp</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ui yellow zuil card">
                    <div class="image">
                        <img alt="" src="img/zuil.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Zuil</h2>
                        <div class="ui divider"></div>
                        <div class="ui icon nood hidden warning message">
                            <i class="exclamation triangle icon"></i>
                            <div class="content">
                                <div class="header">NOOD!</div>
                                <span>Mary heeft de noodalarm gedrukt!</span>
                            </div>
                        </div>
                        <div class="ui icon brand hidden warning message">
                            <i class="exclamation triangle icon"></i>
                            <div class="content">
                                <div class="header">BRAND!</div>
                                <span>Let op: er is kans op brand!</span>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ui olive koelkast card">
                    <div class="image">
                        <img alt="" src="img/koelkast.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Koelkast</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided three column grid">
                                <div class="column deur">
                                    <p><b>Deur</b></p>
                                    <p><span></span></p>
                                </div>
                                <div class="column">
                                    <p><b>Temp. Binnen</b></p>
                                    <p><i style="color:#8b0000" class="fitted large thermometer half icon"></i> <span id="fridge-temp-2">15</span>&#8451;</p>
                                </div>
                                <div class="column">
                                    <p><b>Temp. Buiten</b></p>
                                    <p><i style="color:#8b0000" class="fitted large thermometer half icon"></i> <span id="fridge-temp-1">15</span>&#8451;</p>
                                </div>
                            </div>
                        </div>
                        <div class="ui hidden warning message">
                            <i class="close icon"></i>
                            <div class="header">Koelkast alarm</div>
                            <p>De koelkast deur staat nog open</p>
                        </div>
                    </div>
                </div>

                <div class="ui green deur card">
                    <div class="image">
                        <img alt="" src="img/deur.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Deur</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided three column grid">
                                <div class="column deur">
                                    <p><b>Deur</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                                <div class="column outside lamp">
                                    <p><b>Buiten Lamp</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="ui hidden info message">
                            <div class="header">Deurbel</div>
                            <p>Er staat iemand voor de deur!</p>
                        </div>
                    </div>
                </div>

                <div class="ui teal bed card">
                    <div class="image">
                        <img alt="" src="img/bed.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Bed</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided three column grid">
                                <div class="column lamp">
                                    <p><b>Lamp</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input type="checkbox">
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="ui blue muur card">
                    <div class="image">
                        <img alt="" src="img/muur.png"/>
                    </div>
                    <div class="content">
                        <h2 class="header">Muur</h2>
                        <div class="ui divider"></div>
                        <div class="description">
                            <div class="ui divided two column grid">
                                <div class="column lamp">
                                    <p><b>Lamp</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                                <div class="column dimmer">
                                    <p><b>Dimmer</b></p>
                                    <div class="ui mini buttons">
                                        <div class="ui icon minus basic button"><i class="minus icon"></i></div>
                                        <div class="ui icon plus basic button"><i class="plus icon"></i></div>
                                    </div>
                                </div>
                                <div class="column screen">
                                    <p><b>Scherm</b></p>
                                    <div class="ui fitted toggle checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                                <div class="column disco">
                                    <p><b>Discostand</b></p>
                                    <div class="ui fitted slider checkbox">
                                        <input name="" type="checkbox" value=""/>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

            </div>

        </div>

    </body>
</html>
