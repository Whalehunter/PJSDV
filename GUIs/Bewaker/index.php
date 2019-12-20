<!doctype html>
<html lang="nl">
  <head>
    <title>Control panel</title>
    <meta charset="UTF-8"/>
    <link rel="stylesheet" type="text/css" href="semantic/dist/semantic.min.css">
    <script src="https://code.jquery.com/jquery-3.1.1.min.js"></script>
    <script src="semantic/dist/semantic.min.js"></script>
    <script>
    $(document).ready(function(){
         $('.bed .lamp .checkbox').checkbox({
             onChange: function() {
                 $.ajax('command.php', {
                     dataType: 'json',
                     data: {
                         id:'x',
                         msg:'d'
                     },
                     error: function(data) {
                         $('.bed .lamp .checkbox').checkbox('uncheck');
                     }
                 }).done(function(data){
                     if (data.success) {
                         let toggler = 'uncheck';
                         if (data.lamp === '1')
                             toggler = 'check'

                         setTimeout(function() {
                             $('.bed .lamp .checkbox').checkbox(toggler);
                         }, 100);
                     }
                 });
             }
         });

         // function request(name, value, button) {
         //     button.checkbox('set disabled');
         //     $.ajax('command.php', {
         //         dataType: 'json',	/* Moet text zijn indien text */
         //         data: {
         //             name: name,
         //             val: value,
         //         },
         //         success: function() {
         //             button.checkbox('set enabled');
         //         },
         //         error: function(obj, reason, err) {
         //             console.log(reason);
         //             console.log(err);
         //             if (button.checkbox('is checked'))
         //                 button.checkbox('set unchecked');
         //             else
         //                 button.checkbox('set checked');
         //             button.checkbox('set enabled');
         //         }
         //     });
         // }

         // $('.koelkast .deur .ui.checkbox').checkbox();

         // let items = {
         //     koelkast: [
         //         'koeler'
         //     ],
         //     bed: [
         //         'lamp',
         //         'alarm'
         //     ],
         //     deur: [
         //         'outside.lamp',
         //         'inside.lamp',
         //         'outside.switch',
         //         'inside.switch'
         //     ],
         //     muur: [
         //         'lamp',
         //     ]
         // };

         // Object.keys(items).forEach(function(meubel) {
         //     items[meubel].forEach(function(apparaat)) {
         //         $('.'+meubel+' .'+ apparaat +' .ui.checkbox').checkbox({
         //             onChecked: function() {
         //                 request(meubel, apparaat+'=true', $(this));
         //             },
         //             onUnchecked: function() {
         //                 request(meubel, apparaat+'=false', $(this));
         //             }
         //         });
         //     }
         // });

      });
    </script>
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
                <div class="column">
                  <p><b>Lamp</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column">
                  <p><b>Bewegingdetector</b></p>
                  <p><span id="bewegingssensor">Aan</span></p>
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
                <div class="column">
                  <p><b>Trillen</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column">
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
              <div class="ui divided three column grid">
                <div class="column">
                  <p><b>Licht</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column">
                  <p><b>Rookmelder</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column">
                  <p><b>Zoemer</b></p>
                  <div class="ui fitted toggle checkbox">
                    <input name="" type="checkbox" value=""/>
                  </div>
                </div>
              </div>
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
                  <div class="ui fitted toggle disabled checkbox">
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
              <div class="ui divided two column grid">
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
                  <p><b>Buiten Schakelaar</b></p>
                  <div class="ui fitted toggle checkbox">
                      <input name="" type="checkbox" value=""/>
                  </div>
                </div>
                <div class="column inside switch">
                    <p><b>Binnen Schakelaar</b></p>
                    <div class="ui fitted toggle checkbox">
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

    <div class="ui very padded bottom attached container segment">
      <div class="ui footer">
        copyright motherfuckers
      </div>
    </div>
  </body>
</html>
