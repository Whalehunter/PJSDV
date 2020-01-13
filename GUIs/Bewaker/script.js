// Bij page reload alle ajax requests cancellen, zodat de pagina een beetje snel verversen kan.
var msgs = [];
window.addEventListener('beforeunload', ()=>{
    msgs.forEach((req)=>{
        req.abort();
    });
});

function msg(txt, hook) {
    msgs.push($.ajax('command.php', {
        dataType: 'json',
        data: {
            id: 'x',            // Bewaker GUI ID
            msg:txt,
        },
        error: (data) => {
            console.log(data);
        }
    }).done((data)=>{hook(data)}));
}

function afhandel(data){if(!data.success)console.log(data)}

$(document).ready(()=>{

    /* SCHEMERLAMP */
    $('.schemerlamp .lamp .checkbox').checkbox({
        onChecked:()=>{msg('sla',afhandel)},
        onUnchecked:()=>{msg('slu',afhandel)}
    });

    $('.schemerlamp .disco .checkbox').checkbox({
        onChecked:()=>{msg('sda',afhandel)},
        onUnchecked:()=>{msg('sdu',afhandel)}
    });

    /* STOEL */
    $('.stoel .trillen .checkbox').checkbox();

    /* ZUIL */
    $('.zuil .zoemer .checkbox').checkbox({
        onChecked:()=>{msg('fza',(data)=>{if(!data.success)console.log(data)})},
        onUnchecked:()=>{msg('fzu',(data)=>{console.log(data)})}
    });

    $('.zuil .brandalarm .checkbox').checkbox({
        onChecked:()=>{msg('fba',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('fbu',(data)=>{console.log(data)})}
    });

    $('.zuil .noodalarm .checkbox').checkbox({
        onChecked:()=>{msg('fna',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('fnu',(data)=>{console.log(data)})}
    });

    /* KOELKAST */
    $('.koelkast .deur .checkbox').checkbox();
    $('.koelkast .koeler .checkbox').checkbox();

    /* DEUR */
    $('.deur .inside.switch .checkbox').checkbox();

    $('.deur .outside.switch .checkbox').checkbox();
    $('.deur .inside.lamp .checkbox').checkbox({
        onChecked:()=>{msg('dla',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('dlu',(data)=>{console.log(data)})},
    });

    $('.deur .outside.lamp .checkbox').checkbox({
        onChecked:()=>{msg('dba',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('dbu',(data)=>{console.log(data)})},
    });
    $('.deur .deur .checkbox').checkbox({
        onChecked:()=>{msg('do',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('ds',(data)=>{console.log(data)})}
    });

    $('.bed .alarm .checkbox').checkbox();
    $('.bed .lamp .checkbox').checkbox({
        onChange:()=>{}
    });

    $('.muur .lamp .checkbox').checkbox();


    /* Update interval */
    msg('-dsf', updateElements);

    function updateElements(data) {

        if (!data) {
            return;
        }

        data.forEach((key)=>{
            /* SCHEMERLAMP */
            if (key.Schemerlamp) {
                let s = key.Schemerlamp, c = 'set checked';
                if (!s.Rood && !s.Groen && !s.Blauw) {
                    c = 'set unchecked';
                }
                $('.schemerlamp .lamp .checkbox').checkbox(c);
                if (!s.Beweging && $('#bewegingssensor').hasClass('check')) {
                    $('#bewegingssensor').removeClass('green check icon').addClass('red close icon');
                } else if (s.Beweging && $('#bewegingssensor').hasClass('close')){
                    $('#bewegingssensor').removeClass('red close icon').addClass('green check icon');
                }
                $('.schemerlamp .disco .checkbox').checkbox(s.Disco ? 'set checked' : 'set unchecked');
            }

            /* ZUIL */
            if (key.Zuil) {
                let z = key.Zuil, c = 'set checked';
                if (!z.Zoemer) c = 'set unchecked';
                $('.zuil .zoemer .checkbox').checkbox(c);
                if (!z.Lamp) c = 'set unchecked'; else c = 'set checked';
                $('.zuil .lamp .checkbox').checkbox(c);
                if (!z.Brandalarm) c = 'set unchecked'; else c = 'set checked';
                $('.zuil .brandalarm .checkbox').checkbox(c);
                if (!z.Noodalarm) c = 'set unchecked'; else c = 'set checked';
                $('.zuil .noodalarm .checkbox').checkbox(c);

                /* BRANDALARM */
                if (z.Brandalarm) {
                    $('.zuil .brand.message').show();
                } else {
                    $('.zuil .brand.message').hide();
                }

                if (z.Noodalarm) {
                    $('.zuil .nood.message').show();
                } else {
                    $('.zuil .nood.message').hide();
                }
                $('.zuil .gasmelder-waarde').text(z.Gasmeter);
            }

            /* DEUR */
            if (key.Deur) {
                let buitenKnop = '.outside.switch', binnenKnop = '.inside.switch', buitenLed = '.outside.lamp', binnenLed = '.inside.lamp', deur = '.deur';
                let d = key.Deur, c = 'set checked';

                if (!d.Binnenknop) c = 'set unchecked';
                setCheckbox('.deur '+binnenKnop, c);

                if(d.Buitenknop) c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+buitenKnop, c);

                if (d.Binnenled) c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+binnenLed, c);

                if (d.Buitenled) c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+buitenLed, c);

                if (d.Deur == 'open') c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+deur, c);
            }

        });

        setTimeout(()=>{msg('-dsf', updateElements)}, 100);
    }

    function setCheckbox(where, action) {
        $(where+" .checkbox").checkbox(action);
    }

});
