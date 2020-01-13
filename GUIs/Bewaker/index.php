<!doctype html>
<html lang="nl">
  <head>
    <title>Control panel</title>
    <meta charset="UTF-8"/>
    <link rel="stylesheet" type="text/css" href="semantic/dist/semantic.min.css">
    <script src="jquery-3.4.1.min.js"></script>
    <script src="semantic/dist/semantic.min.js"></script>
    <script src="script.js"></script>
  </head>
  <body>

    <div class="ui top attached very padded segment">
      <h1 class="ui header"><i class="desktop icon"></i><div class="content">Control Panel<div class="sub header">Made by van Adrichem, Hooghuis, Meissen, Olsthoorn</div></div></h1>

    </div>
    <div class="ui attached very padded container segment">
      <div class="ui cards">

        <div class="ui schemerlamp card">
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
                <div class="column movement">
                  <p><b>Bewegingdetector</b></p>
                  <i class="large icons">
                      <i class="street view icon"></i>
                      <i id="bewegingssensor" class="top right corner green check icon"></i>
                  </i>
                </div>
                <div class="column disco">
                    <p><b>Discostand</b></p>
                    <div class="ui fitted slider checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
                <div class="column brightness">
                    <p><b>Lamp Helderheid</b></p>
                    <div class="ui mini buttons">
                        <div class="ui icon basic button"><i class="minus icon"></i></div>
                        <div class="ui icon basic button"><i class="plus icon"></i></div>
                    </div>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="ui stoel card">
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
                <div class="column plek">
                  <p><b>Plek</b></p>
                  <p>Beschikbaar</p>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="ui zuil card">
          <div class="image">
            <img alt="" src="img/zuil.png"/>
          </div>
          <div class="content">
            <h2 class="header">Zuil</h2>
            <div class="ui divider"></div>
            <div class="description">
              <div class="ui divided two column grid">
                <div class="column gasmelder">
                  <p><b>Gasmelder</b></p>
                  <p><span class="gasmelder-waarde">0</span></p>
                </div>
                <div class="column zoemer">
                    <p><b>Zoemer</b></p>
                    <div class="ui fitted slider checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
                <div class="column noodalarm">
                    <p><b>Noodalarm</b></p>
                    <div class="ui fitted slider checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
                <div class="column brandalarm">
                    <p><b>Brandalarm</b></p>
                    <div class="ui fitted slider checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
              </div>
            </div>
          </div>
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

        <div class="ui koelkast card">
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
                  <div class="ui fitted slider disabled checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column koeler">
                  <p><b>Koeler</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column">
                  <p><b>Temperatuur</b></p>
                  <p><i style="color:#8b0000" class="fitted large thermometer half icon"></i> <span id="fridge-temp">15</span>&#8451;</p>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="ui deur card">
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
                <div class="column inside lamp">
                    <p><b>Binnen Lamp</b></p>
                    <div class="ui fitted toggle checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
                <div class="column outside switch">
                  <p><b>Buiten Knop</b></p>
                  <div class="ui fitted slider checkbox">
                      <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column inside switch">
                    <p><b>Binnen Knop</b></p>
                    <div class="ui fitted slider checkbox">
                        <input name="" type="checkbox" value=""/>
                    </div>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="ui bed card">
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
                <div class="column alarm">
                  <p><b>Alarm</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input type="checkbox">
                  </div>
                </div>
                <div class="column">
                  <p><b>Plek</b></p>
                  <p>Bezet</p>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div class="ui muur card">
        <div class="image">
          <img alt="" src="img/muur.png"/>
        </div>
        <div class="content">
          <h2 class="header">Muur</h2>
          <div class="ui divider"></div>
          <div class="description">
            <div class="ui divided three column grid">
              <div class="column lamp">
                <p><b>Lamp</b></p>
                <div class="ui fitted toggle checkbox">
                  <input name="" type="checkbox" value=""/>
                </div>
              </div>
              <div class="column dimmer">
                <p><b>Dimmer</b></p>
                <div class="ui mini buttons">
                  <div class="ui icon basic button"><i class="minus icon"></i></div>
                  <div class="ui icon basic button"><i class="plus icon"></i></div>
                </div>
              </div>
              <div class="column screen">
                <p><b>Scherm</b></p>
                <div class="ui mini buttons">
                  <div class="ui icon basic button"><i class="minus icon"></i></div>
                  <div class="ui icon basic button"><i class="plus icon"></i></div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>

  </body>
</html>
