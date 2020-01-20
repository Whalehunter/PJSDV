var msgs = [];
APPARATEN = '-sdfzkym'
window.addEventListener('beforeunload', ()=>{
    msgs.forEach((req)=>{
        req.abort();
    });
});

function msg(txt, fun=null) {
    msgs.push($.ajax('command.php', {
        dataType: 'json',
        data: {
            id: 'x',
            msg:txt,
        },
        error:(data) => {
            console.log(data);
        }
    }).done(fun));              // TODO testen
}

$(document).ready(()=>{

    // MARY NOOD KNOP
    $('#mary-nood-knop').click(()=>{msg('fna')});

    // Schemerlamp
    $('.schemerlamp .lamp .checkbox').checkbox({
        onChecked:()=>{msg('sla')}, // Schemerlamp lamp aan
        onUnchecked:()=>{msg('slu')} // schemerlamp lamp uit
    });

    $('.schemerlamp .disco .checkbox').checkbox({
        onChecked:()=>{msg('sda')}, // Discostand van schemerlamp aan
        onUnchecked:()=>{msg('sdu')}, // Discostand van schemerlamp uit
    });

    // Stoel
    $('.stoel .trillen .checkbox').checkbox({
        onChecked:()=>{msg('zta')}, // stoel trilstand aan
        onUnchecked:()=>{msg('ztu')}, // stoel trilstand uit
    });
    $('.stoel .lamp .checkbox').checkbox({
        onChecked:()=>{msg('zla')}, // lamp van stoel aan
        onUnchecked:()=>{msg('zlu')} // lamp van stoel uit
    });

    // Zuil
    $('.zuil .zoemer .checkbox').checkbox({
        onChecked:()=>{msg('fza')}, // zoemer op zuil aanzetten
        onUnchecked:()=>{msg('fzu')} // zoemer op zuil uitzetten
    });

    // Koelkast
    $('.koelkast .koeler .checkbox').checkbox(); // Checkbox mogelijk maken
    $('.koelkast .message .close').click(()=>{
        $('.koelkast .message').hide(); // verberg message
        msg('k');               // stop koelkast open deur alarm
    })

    // Deur
    $('.deur .outside.lamp .checkbox').checkbox({
        onChecked:()=>{msg('dba')}, //
        onUnchecked:()=>{msg('dbu')}
    });
    $('.deur .deur .checkbox').checkbox({
        onChecked:()=>{msg('do')},
        onUnchecked:()=>{msg('ds')}
    });

    // Bed
    $('.bed .lamp .checkbox').checkbox({
        onChecked:()=>{msg('yla')},
        onUnchecked:()=>{msg('ylu')}
    });

    $('.bed .message .close').click(()=>{$('.bed .message').hide()});

    // Muur
    $('.muur .lamp .checkbox').checkbox({
        onChecked:()=>{msg('mla')},
        onUnchecked:()=>{msg('mlu')}
    });

    $('.muur .disco .checkbox').checkbox({
        onChecked:()=>{msg('mda')},
        onUnchecked:()=>{msg('mdu')}
    });

    $('.muur .screen .checkbox').checkbox({
        onChecked:()=>{msg('mra')},
        onUnchecked:()=>{msg('mru')}
    });

    $('.muur .dimmer .plus.button').click(()=>{msg('mbu')});
    $('.muur .dimmer .minus.button').click(()=>{msg('mbd')});


    /* UPDATE WAARDEN */
    msg(APPARATEN, updateElements);

    function updateElements(data) {
        if (!data) return;

        data.forEach((d)=>{
            let c = 'set checked', u = 'set unchecked', t;
            if (d.Schemerlamp) {
                let s = d.Schemerlamp;
                if (!s.Rood && !s.Groen && !s.Blauw) t = u;
                else t = c;
                $('.schemerlamp .lamp .checkbox').checkbox(t);

                $('.schemerlamp .disco .checkbox').checkbox(s.Disco ? c : u);

                $('.schemerlamp-helderheid').text((s.Helderheid ? s.Helderheid : '0') + "%");
            }

            if (d.Stoel) {
                let s = d.Stoel;
                $('.stoel .lamp .checkbox').checkbox(s.Lamp ? c : u);
                $('.stoel .trillen .checkbox').checkbox(s.Massage ? c : u);
            }

            $('.zuil .zoemer .checkbox').checkbox(d.Zuil && d.Zuil.Zoemer ? c : u);

            if (d.Koelkast) {
                let k = d.Koelkast;
                $('.koelkast .deur span').text(k.Deur ? "Dicht" : "Open");
                $('#fridge-temp-2').text(k.m1.toFixed(2));
                $('#fridge-temp-1').text(k.m2.toFixed(2));
                if (k.Alarm) {
                    $('.koelkast .message').show();
                } else {
                    $('.koelkast .message').hide();
                }
            }

            if (d.Deur) {
                let r = d.Deur;
                if (r.Deur === "open") {
                    $('.deur .info.message').hide();
                    t = c;
                } else t = u;
                $('.deur .deur .checkbox').checkbox(t);

                $('.deur .outside.lamp .checkbox').checkbox(r.Buitenled?c:u);
            }

            if (d.Bed) $('.bed .lamp .checkbox').checkbox(d.Bed.Lamp?c:u);

            if (d.Muur) {
                let m = d.Muur;
                $('.muur .lamp .checkbox').checkbox(isAan(m.LED0)?c:u);
                $('.muur .screen .checkbox').checkbox(m.Raam?c:u);
                $('.muur .disco .checkbox').checkbox(m.Disco?c:u);
            }

        });

        setTimeout(()=>{msg(APPARATEN, updateElements)}, 100);
    }
});

function isAan(led){
    if (!led.R && !led.B && !led.G)
        return false;
    return true;
}
