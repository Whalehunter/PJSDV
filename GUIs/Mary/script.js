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
    // Schemerlamp
    $('.schemerlamp .lamp .checkbox').checkbox({
        onChecked:()=>{msg('sla')},
        onUnchecked:()=>{msg('slu')}
    });

    $('.schemerlamp .disco .checkbox').checkbox({
        onChecked:()=>{msg('sda')},
        onUnchecked:()=>{msg('sdu')},
    });

    // Stoel
    $('.stoel .trillen .checkbox').checkbox({
        onChecked:()=>{msg('zta')},
        onUnchecked:()=>{msg('ztu')},
    });
    $('.stoel .lamp .checkbox').checkbox({
        onChecked:()=>{msg('zla')},
        onUnchecked:()=>{msg('zlu')}
    });

    // Zuil
    $('.zuil .zoemer .checkbox').checkbox({
        onChecked:()=>{msg('fza')},
        onUnchecked:()=>{msg('fzu')}
    });

    // Koelkast
    $('.koelkast .deur .checkbox').checkbox();
    $('.koelkast .koeler .checkbox').checkbox();

    // Deur
    $('.deur .outside.lamp .checkbox').checkbox({
        onChecked:()=>{msg('dba')},
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

            $('.bed .lamp .checkbox').checkbox(d.Bed && d.Bed.Lamp ? c : u);

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
