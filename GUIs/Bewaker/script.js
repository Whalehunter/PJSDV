// Bij page reload alle ajax requests cancellen, zodat de pagina een beetje snel verversen kan.
var msgs = [];
window.addEventListener('beforeunload', ()=>{
    clearInterval(interval);
    msgs.forEach((req)=>{
        req.abort();
    });
});

function msg(txt, hook) {
    msgs.push($.ajax('command.php', {
        dataType: 'json',
        data: {
            id: 'x',
            msg:txt,
        },
        error: (data) => {
            console.log(data);
        }
    }).done((data)=>{hook(data)}));
}

$(document).ready(()=>{

    $('.message .close').on('click', ()=>{
        $(this).closest('.message').removeClass('message').addClass('hidden message');
    });

    $('.schemerlamp .lamp .checkbox').checkbox();

    $('.stoel .trillen .checkbox').checkbox();

    $('.zuil .licht .checkbox').checkbox();
    $('.zuil .rookmelder .checkbox').checkbox();
    $('.zuil .zoemer .checkbox').checkbox();

    $('.koelkast .deur .checkbox').checkbox();
    $('.koelkast .koeler .checkbox').checkbox();

    $('.deur .inside.switch .checkbox').checkbox();

    $('.deur .outside.switch .checkbox').checkbox();
    $('.deur .inside.lamp .checkbox').checkbox();
    $('.deur .outside.lamp .checkbox').checkbox();
    $('.deur .deur .checkbox').checkbox({
        onChecked:()=>{msg('xo',(data)=>{console.log(data)})},
        onUnchecked:()=>{msg('xx',(data)=>{console.log(data)})}
    });

    $('.bed .alarm .checkbox').checkbox();
    $('.bed .lamp .checkbox').checkbox({
        onChange:()=>{
            msg('d', (data) => {
                if (data) {
                    let toggler = 'uncheck';
                    if (data.lamp === '1') toggler = 'check';
                    setTimeout(()=>{$('.bed .lamp .checkbox').checkbox(toggler)}, 100);
                }
            });
        }
    });

    $('.muur .lamp .checkbox').checkbox();

    /* Update interval */
    interval = setInterval(function() {
        msg('dsf', updateElements);
    }, 100);

    function updateElements(data) {

        /* SCHEMERLAMP */
        if (data.Schemerlamp) {
            let s = data.Schemerlamp, c = 'set checked';
            if (!s.Lamp) {
                c = 'set unchecked';
            }
            $('.schemerlamp .lamp .checkbox').checkbox(c);
            $('.schemerlamp .movement #bewegingssensor').text(s.Beweging);
        }

        /* ZUIL */
        if (data.Zuil) {
            let z = data.Zuil, c = 'set checked';
            if (!z.Zoemer) c = 'set unchecked';
            $('.zuil .zoemer .checkbox').checkbox(c);
            c = 'set checked';
            if (!z.Lamp) c = 'set unchecked';
            $('.zuil .lamp .checkbox').checkbox(c);

            /* BRANDALARM */
            if (z.Brandalarm) {
                $('.zuil .warning.message .header').text('BRAND!');
                $('.zuil .warning.message span').text('Let op: er is kans op brand!');
                $('.zuil .warning.message').show();
            } else {
                $('.zuil .warning.message .header').text();
                $('.zuil .warning.message span').text();
                $('.zuil .warning.message').hide();
            }

            /* NOODALARM */
            if (z.Noodalarm) {
                $('.zuil .warning.message .header').text('NOOD!');
                $('.zuil .warning.message span').text('Mary heeft de noodalarm gedrukt!');
                $('.zuil .warning.message').show();
            } else {
                $('.zuil .warning.message .header').text();
                $('.zuil .warning.message span').text();
                $('.zuil .warning.message').hide();
            }
            $('.zuil .gasmelder-waarde').text(z.Gasmeter);
        }

        /* DEUR */
        if (data.Deur) {
            let buitenKnop = '.outside.switch', binnenKnop = '.inside.switch', buitenLed = '.outside.lamp', binnenLed = '.inside.lamp', deur = '.deur';
            let d = data.Deur, c = 'set checked';

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
    }

    function setCheckbox(where, action) {
        $(where+" .checkbox").checkbox(action);
    }


    // function request(name, value, button) {
    //     button.checkbox('set disabled');
    //     $.ajax('command.php', {
    //         dataType: 'json',        /* Moet text zijn indien text */
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
