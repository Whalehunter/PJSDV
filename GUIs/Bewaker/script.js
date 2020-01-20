// Bij page reload alle ajax requests cancellen, zodat de pagina een beetje snel verversen kan.
var msgs = [];
APPARATEN = '-sdfzkym';
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
    $('.stoel .trillen .checkbox').checkbox({
        onChecked:()=>{msg('zta',(data)=>{if(!data.success)console.log(data)})},
        onUnchecked:()=>{msg('ztu',(data)=>{console.log(data)})}
    });

    $('.stoel .lamp .checkbox').checkbox({
        onChecked:()=>{msg('zla',(data)=>{if(!data.success)console.log(data)})},
        onUnchecked:()=>{msg('zlu',(data)=>{console.log(data)})}
    })

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

    /* BED */
    $('.bed .lamp .checkbox').checkbox({
        onChecked:()=>{msg('yla',(data)=>{if(!data.success)console.log(data)})},
        onUnchecked:()=>{msg('ylu',(data)=>{console.log(data)})}
    });

    $('.bed .message .close').click(()=>{$('.bed .message').hide()});

    /* MUUR */
    $('.muur .lamp .checkbox').checkbox({
        onChecked:()=>{msg('mla',afhandel)},
        onUnchecked:()=>{msg('mlu',afhandel)}
    });

    $('.muur .disco .checkbox').checkbox({
        onChecked:()=>{msg('mda',afhandel)},
        onUnchecked:()=>{msg('mdu',afhandel)}
    });

    $('.muur .screen .checkbox').checkbox({
        onChecked:()=>{msg('msa',afhandel)},
        onUnchecked:()=>{msg('msu',afhandel)}
    });


    /* Update interval */
    msg(APPARATEN, updateElements);

    function updateElements(data) {

        if (!data) {
            return;
        }

        inbraak = {};

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

                let sHelderheid = '%';
                if (s.Helderheid) {
                    sHelderheid = s.helderheid + ' %';
                }
                $('.schemerlamp-helderheid').text(sHelderheid);

                inbraak.schemerlamp = s.Beweging;
            }

            /* STOEL */
            if (key.Stoel) {
                let s = key.Stoel;
                if (s.Knop) {
                    $('.stoel .knop .checkbox').checkbox('set checked');
                } else {
                    $('.stoel .knop .checkbox').checkbox('set unchecked');
                }
                if (s.Lamp) {
                    $('.stoel .lamp .checkbox').checkbox('set checked');
                } else {
                    $('.stoel .lamp .checkbox').checkbox('set unchecked');
                }
                if (s.Massage) {
                    $('.stoel .trillen .checkbox').checkbox('set checked');
                } else {
                    $('.stoel .trillen .checkbox').checkbox('set unchecked');
                }
                if (s.Drukplaat) {
                    $('.stoel .plek span').text("Bezet");
                    $('.stoel .trillen input').removeAttr('disabled');
                } else {
                    $('.stoel .plek span').text("Beschikbaar");
                    $('.stoel .trillen input').attr('disabled', 'disabled');
                }
            }

            /* ZUIL */
            if (key.Zuil) {
                let z = key.Zuil, c = 'set checked';
                if (!z.Zoemer) c = 'set unchecked';
                $('.zuil .zoemer .checkbox').checkbox(c);
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

            /* KOELKAST */
            if (key.Koelkast) {
                let k = key.Koelkast;
                if (k.Deur) {
                    $('.koelkast .deur span').text("Dicht");
                } else {
                    $('.koelkast .deur span').text("Open");
                }

                if (k.Koelelement) {
                    $('.koelkast .koeler .checkbox').checkbox('set checked');
                } else {
                    $('.koelkast .koeler .checkbox').checkbox('set unchecked');
                }

                if (k.Fan) {
                    $('.koelkast .fan .checkbox').checkbox('set checked');
                } else {
                    $('.koelkast .fan .checkbox').checkbox('set unchecked');
                }

                $('#fridge-temp-2').text(k.m1.toFixed(2));
                $('#fridge-temp-1').text(k.m2.toFixed(2));
            }

            /* DEUR */
            if (key.Deur) {
                let buitenKnop = '.outside.switch', binnenKnop = '.inside.switch', buitenLed = '.outside.lamp', binnenLed = '.inside.lamp', deur = '.deur';
                let d = key.Deur, c = 'set checked';

                if (!d.Binnenknop) c = 'set unchecked';
                setCheckbox('.deur '+binnenKnop, c);

                if(d.Buitenknop) {
                    c = 'set checked';
                    $('.deur .info.message').show();
                } else {
                    c = 'set unchecked';
                }
                setCheckbox('.deur '+buitenKnop, c);

                if (d.Binnenled) c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+binnenLed, c);

                if (d.Buitenled) c = 'set checked';
                else c = 'set unchecked';
                setCheckbox('.deur '+buitenLed, c);

                if (d.Deur == 'open') {
                    c = 'set checked';
                    $('.deur .info.message').hide();
                } else c = 'set unchecked';
                setCheckbox('.deur '+deur, c);
            }

            /* BED */
            if (key.Bed) {
                let b = key.Bed;

                if (b.Lamp) {
                    $('.bed .lamp .checkbox').checkbox('set checked');
                } else {
                    $('.bed .lamp .checkbox').checkbox('set unchecked');
                }

                if (b.knop) {
                    $('.bed .knop .checkbox').checkbox('set checked');
                } else {
                    $('.bed .knop .checkbox').checkbox('set unchecked');
                }

                $('.bed .plek span').text(b.drukSensor);
                inbraak.bed = b.drukSensor;
            }

            /* MUUR */
            if (key.Muur) {
                let m = key.Muur,
                    aan = isAan(m.LED0);
                $('.muur .screen .checkbox').checkbox(m.raam?'set checked':'set unchecked');
                $('.muur .lamp .checkbox').checkbox(aan?'set checked':'set unchecked');
                $('.muur .disco .checkbox').checkbox(aan&&isDisco(m.LED0)?'set checked':'set unchecked');
            }

        });

        if (inbraak.bed === "Bezet" && inbraak.schemerlamp === 1) {
            $('.bed .message').show();
        }

        setTimeout(()=>{msg(APPARATEN, updateElements)}, 100);
    }

    function setCheckbox(where, action) {
        $(where+" .checkbox").checkbox(action);
    }


    function isAan(led){
        if (!led.R && !led.B && !led.G)
            return false;
        return true;
    }

    function isDisco(led) {
        if ((led.R || led.B || led.G) && (!led.R || !led.B || !led.G))
            return true;
        return false;
    }

});
